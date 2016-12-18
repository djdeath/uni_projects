require 'graph'
require 'timecounter'
require 'vector'

require 'display'

module ConvexEnv
        
        def draw_convexenv(display)
                points = sorted_points
                
                self.nb_sommet.times do |i|
                        p1 = points[i]
                        p2 = points[(i + 1) % self.nb_sommet]
                        display.draw_edge(p1, p2)
                end
        end
        
        #######
        private
        #######
        
        def sorted_points
                vectors = []
                center = self.center_point
                vref = Vector.new(center, self.high_left_point)
                
                self.points.each do |p|
                        vectors << Vector.new(center, p)
                end
                
                vectors.sort! do |v1, v2|
                        a1 = vref.angle(v1)
                        a2 = vref.angle(v2)
                        
                        -1 if a1 < a2
                        1 if a1 > a2
                        -1 if v1.distance > v2.distance
                        1
                end
                
                puts vectors[0].p2.class
                
                points = []
                vectors.each do |v|
                        points << v.p2
                        puts v.p2.class
                end
                
                return points;
        end
        
        def graham
                
        end
        
end
