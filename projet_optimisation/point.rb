require 'gnomecanvas2'

class Point
        attr_accessor :x, :y
        
        def initialize(x, y)
                @x = x
                @y = y
        end
        
        def distance(p)
                x2 = (@x - p.x)
                y2 = (@y - p.y)
                
                return Math.sqrt(x2 * x2 + y2 * y2)
        end
        
        def vector(p)
                return Vector.new(self, p)
        end
        
        def to_s
                "#{x}x#{y}"
        end
        
end
