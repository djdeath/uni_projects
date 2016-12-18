require 'gtk2'

require 'point'
require 'graph'

module Map
        
        # Création d'un tableau de point au hasard
        def Map.random_points(width, height, num)
                
                # Nombre de points supérieur au nombre de cases disponible
                if num > width * height
                        raise "Too many points"
                end
                
                map = []
                height.times { map << Array.new(width, 0) }
                points = []
                
                a = 0
                while a < num
                        x = Kernel::rand(width)
                        y = Kernel::rand(height)
                        
                        if map[y][x] == 0
                                map[y][x] = 1
                                points << Point.new(x, y)
                                
                                a = a.next
                        end
                end
                
                return points
        end
        
        # Chargement de fichiers de points
        def Map.read_points(width, height, filename)
                file = File.new(filename, 'r')
                points = []
                
                a = 0
                line = 1
                max_points = width * height
                file.each do |line|
                        if a >= max_points
                                break
                        end
                        t = line.split
                        
                        if t.length == 2
                                x = t[0].to_i
                                y = t[1].to_i
                                if x < 0 or x >= width or y < 0 or y >= height
                                        puts "Point " + t[0] + "x" + t[1] + " is outside the map."
                                else
                                        points << Point.new(x, y)
                                        a = a.next
                                end
                        elsif
                                puts "Line error : " + line.to_s
                        end
                end
                
                return points
        end
        
        def Map.write_points(filename, points)
                file = File.new(filename, 'w')
                
                points.each do |p|
                        file.puts "#{p.x} #{p.y}"
                end
        end
end
