require 'gnome2'
require 'libglade2'

require 'map'
require 'graph'
require 'prim'
require 'status'
require 'preferences'

class Display
        attr_reader :stats, :prefs
        
        POINT_SIZE = 10
        
        def initialize(width, height)
                
                # Points et graphe
                @width = width
                @height = height
                @points = []
                @graph = nil
                
                # partie graphique
                @drawing = false # permet de savoir si l'on dessine à un moment précis
                @fileselector = nil # selecteur de fichier
                
                ui = GladeXML.new("ui.glade")
                
                window = ui['main_window']
                window.signal_connect('destroy') { Gtk.main_quit }
                
                @prefs = Preferences.new(ui)
                @stats = Status.new(ui['treeview'])
                @statusbar = ui['statusbar']
                
                @gc_draw_point = Gdk::GC.new(window.window)
                @gc_draw_edge = Gdk::GC.new(window.window)
                @gc_undraw = window.style.bg_gc(window.state)
                
                @pixmap = Gdk::Pixmap.new(window.window, @width, @height, -1)
                @image = ui['image']
                @image.pixmap = @pixmap
                
                ui['generate_new_map'].signal_connect('activate') do
                        generate_random_points
                end
                ui['generate_new_map_button'].signal_connect('clicked') do
                        generate_random_points
                end
                
                ui['load_map'].signal_connect('activate') do
                        load_points
                end
                ui['load_map_button'].signal_connect('clicked') do
                        load_points
                end
                
                ui['save_map'].signal_connect('activate') do
                        unless @points.empty?
                                file_selector do |filename|
                                        Map.write_points(filename, @points)
                                end
                        end
                end
                
                ui['save_image'].signal_connect('activate') do
                        file_selector do |filename|
                                image = Gdk::Pixbuf.from_drawable(nil, @pixmap, 0, 0, @width, @height)
                                image.save(filename, 'png')
                        end
                end
                
                
                ui['clean_edges'].signal_connect('activate') do
                        clean_edges
                end
                
                
                ui['covering_tree'].signal_connect('activate') do
                        call_euristic { # @graph.draw_tree(self) }
                                @graph.draw_convexenv(self)
                        }
                end
                
                ui['covering_euristic'].signal_connect('activate') do
                        call_euristic { @graph.draw_route(self) }
                end
                
                ui['farest_insertion_euristic'].signal_connect('activate') do
                        call_euristic { @graph.draw_far(self) }
                end
                
                ui['separation_evaluation'].signal_connect('activate') do
                        call_euristic { @graph.draw_sepval(self) }
                end
                
                ui['simulated_annealing'].signal_connect('activate') do
                        call_euristic { @graph.draw_annealing(self) }
                end
                
                ui['elastic'].signal_connect('activate') do
                        call_euristic { @graph.draw_elastic(self) }
                end
                
                ui['preferences'].signal_connect('activate') { @prefs.toggle_show }
                
                ui['about'].signal_connect('activate') { about }
                
                ui['quit'].signal_connect('activate') do
                        quit
                        window.destroy
                end
                
                clear
        end
        
        # Redessiner de la fenêtre
        def drawing?
                return @drawing
        end
        
        def set_drawing
                @drawing = true
        end
        
        def unset_drawing
                @drawing = false
        end
        
        def redraw
                while (Gtk.events_pending?)
                        Gtk.main_iteration
                end
        end
        
        # Afficher du texte
        def set_text(text)
                @statusbar.push(text)
        end
        
        # Effacer les arêtes
        def clear
                @pixmap.draw_rectangle(@gc_undraw, true, 0, 0, -1, -1)
                @image.queue_draw()
        end
        
        def clean_edges
                clear
                @points.each do |p|
                        draw_point(p)
                end
        end
        
        # Dessiner en passant les ids
        def draw_point_id(point)
                draw_point(@points[point])
        end
        
        def delete_point_id(point)
                delete_point(@points[point])
        end
        
        def draw_edge_ids(point1, point2)
                draw_edge(@points[point1], @points[point2])
        end
        
        def delete_edge_ids(point1, point2)
                delete_edge(@points[point1], @points[point2])
        end
        
        # Dessiner en passant les coordonnées
        def draw_edge(point1, point2)
                @pixmap.draw_line(@gc_draw_edge,
                                  point1.x.to_i, point1.y.to_i,
                                  point2.x.to_i, point2.y.to_i)
                @image.queue_draw()
        end
        
        def delete_edge(point1, point2)
                @pixmap.draw_line(@gc_undraw,
                                  point1.x.to_i, point1.y.to_i,
                                  point2.x.to_i, point2.y.to_i)
                draw_point(point1)
                draw_point(point2)
                @image.queue_draw()
        end
        
        def draw_point(point)
                layout = @image.create_pango_layout(@points.index(point).to_s)
                @pixmap.draw_arc(@gc_draw_point, true,
                                 point.x.to_i - POINT_SIZE / 2,
                                 point.y.to_i - POINT_SIZE / 2,
                                 10, 10,
                                 0, 360 * 64)
                if @prefs['draw_cities'] == true
                        @pixmap.draw_layout(@gc_draw_edge,
                                            point.x.to_i + POINT_SIZE,
                                            point.y.to_i - POINT_SIZE / 2,
                                            layout)
                end
        end
        
        def delete_point(point)
                layout = @image.create_pango_layout(@points.index(point).to_s)
                @pixmap.draw_point(@gc_undraw, point.x.to_i, point.y.to_i)
                @pixmap.draw_layout(@gc_undraw,
                                    point.x.to_i,
                                    point.y.to_i,
                                    layout,
                                    nil, nil)
        end
        
        #######
        private
        #######
        
        def load_gcs
                @gc_draw_point.set_rgb_fg_color(@prefs['point_color'])
                @gc_draw_edge.set_rgb_fg_color(@prefs['edge_color'])
        end
        
        def quit
                clean_edges
                clear
        end
        
        def file_selector(&handler)
                if @fs == nil
                        @fs = Gtk::FileSelection.new('Choisissez un fichier.')
                end
                @fs.show
                
                @fs.ok_button.signal_connect('clicked') do
                        @fs.hide
                        handler.call(@fs.filename)
                end
                
                @fs.cancel_button.signal_connect('clicked') do
                        @fs.hide
                end
        end
        
        def call_euristic(&handler)
                unless @points.empty?
                        set_drawing
                        clean_edges
                        handler.call
                        unset_drawing
                end
        end
        
        def about
                unless @about_window != nil
                        arr = Array.new
                        arr[0] = 'Lionel Landwerlin'
                        @about_window = Gnome::About.new('Ruby Graph', '0.0.1', '',
                                                         'Un programme pour les graphes en Ruby.',
                                                         arr, [], nil)
                        @about_window.signal_connect('destroy') { @about_window = nil }
                end
                if @about_window.visible?
                        @about_window.hide
                else
                        @about_window.show
                end
        end
        
        def load_points
                load_gcs
                file_selector do |filename|
                        set_drawing
                        @points = Map.read_points(@width, @height, filename)
                        @graph = Graph.new(@points)
                                clean_edges
                        @statusbar.pop
                        unset_drawing
                end
        end
        
        def generate_random_points
                load_gcs
                set_drawing
                @points = Map.random_points(@width, @height, @prefs['nb_point'])
                @graph = Graph.new(@points)
                clean_edges
                @statusbar.pop
                unset_drawing
        end
        
end
