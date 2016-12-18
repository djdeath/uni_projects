require 'point'

class Vector
        attr_accessor :x, :y, :p1, :p2
        
        def initialize (p1, p2)
                @p1 = p1
                @p2 = p2
                @x = p2.x - p1.x
                @y = p2.y - p1.y
        end
        
        def distance
                return Math.sqrt(x * x + y * y)
        end
        
        def angle(v)
                sc = (x * v.x + y * v.y).to_f
                sc2 = distance * v.distance
                r = sc / sc2
                
                puts "#{sc} / #{sc2} = #{r} !!!!"
                
                return Math.acos(r)
        end
        
end
