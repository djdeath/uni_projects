require 'gnome2'

require 'display'

Gnome::Program.new('Ruby Graph', '0.0.1')

display = Display.new(800, 600)

Gtk.main
