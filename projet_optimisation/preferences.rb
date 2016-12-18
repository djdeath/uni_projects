class Preferences < Hash
        
        def initialize(ui)
                super()
                
                @ui = ui
                @window = @ui['preferences_window']
                
                @window.signal_connect('delete-event') { @window.hide }
                
                # Valeurs par défaut
                add_int_spin('nb_point', 5)
                add_color_button('point_color', Gdk::Color.parse('#ffff00000000'))
                add_color_button('edge_color', Gdk::Color.parse('#000000000000'))
                add_check_button('draw_cities', true)
                
                add_float_spin('annealing_t0', 0.80)
                add_float_spin('annealing_tdec', 0.24)
                
                add_float_spin('elastic_factor_point', 3.0)
                add_float_spin('elastic_k', 1)
                add_float_spin('elastic_kdec', 0.8)
                add_float_spin('elastic_alpha', 1.1)
                add_float_spin('elastic_betha', 0.95)
        end
        
        def toggle_show
                if @window.visible?
                        @window.hide
                else
                        @window.show
                end
        end
        
        #######
        private
        #######
        
        def add_int_spin(desc, val)
                widget = @ui[desc]
                self[desc] = val
                widget.value = val.to_f
                widget.signal_connect('value-changed') do
                        self[desc] = widget.value_as_int
                end
        end
        
        def add_float_spin(desc, val)
                widget = @ui[desc]
                self[desc] = val
                widget.value = self[desc]
                widget.signal_connect('value-changed') do
                        self[desc] = widget.value
                end
        end
        
        def add_check_button(desc, val)
                widget = @ui[desc]
                self[desc] = val
                widget.active = val
                widget.signal_connect('toggled') do
                        self[desc] = widget.active?
                end
        end
        
        def add_color_button(desc, val)
                widget = @ui[desc]
                self[desc] = val
                widget.color = self[desc]
                widget.signal_connect('color-set') do
                        puts "#{desc} color changed"
                        self[desc] = widget.color
                end
        end
        
end
