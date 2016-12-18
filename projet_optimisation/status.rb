require 'gtk2'

class Status
        NAME_COLUMN = 0
        TIME_COLUMN = 1
        WEIG_COLUMN = 2
        
        def initialize(treeview)
                @stats = Hash.new
                @treeview = treeview
                
                @model = Gtk::ListStore.new(String, String, String)
                @treeview.set_model(@model)
                
                column = Gtk::TreeViewColumn.new('méthode',
                                                 Gtk::CellRendererText.new,
                                                 { :text => NAME_COLUMN })
                @treeview.append_column(column)
                column = Gtk::TreeViewColumn.new('temps',
                                                 Gtk::CellRendererText.new,
                                                 { :text => TIME_COLUMN })
                @treeview.append_column(column)
                column = Gtk::TreeViewColumn.new('poids',
                                                 Gtk::CellRendererText.new,
                                                 { :text => WEIG_COLUMN })
                @treeview.append_column(column)
        end
        
        def set_stat(stat, time, weight)
                @stats[stat] = get_new_iter if @stats[stat] == nil
                @stats[stat][NAME_COLUMN] = stat
                @stats[stat][TIME_COLUMN] = time
                @stats[stat][WEIG_COLUMN] = weight.to_s
        end
        
        def clear
                @model.clear
                @stats.clear
        end
        
        #######
        private
        #######
        
        def get_new_iter
                iter = @model.append
        end
end
