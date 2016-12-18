require 'prim'
require 'far'
require 'sepeval'
require 'annealing'
require 'elastic'
require 'convexenv'

class Graph
        attr_reader :nb_sommet, :map, :points
        
        include Prim
        include Far
        include Sepeval
        include Annealing
        include Elastic
        include ConvexEnv
        
        def initialize(points)
                @nb_sommet = points.length
                
                if (@nb_sommet <= 0)
                        raise "A graph with no tops isn't really a graph"
                end
                
                @points = points;
                @map = Array.new(@nb_sommet)
                
                @nb_sommet.times do |i|
                        @map[i] = Array.new(@nb_sommet, 0)
                        
                        # Recopie des valeurs déjà calculées
                        i.times do |j|
                                @map[i][j] = @map[j][i]
                        end
                        
                        # Calcul de la distances entre deux points
                        i.step(@nb_sommet - 1, 1) do |j|
                                @map[i][j] = (points[i].distance(points[j]) * 10).to_i.to_f / 10
                        end
                end
        end
        
        def high_left_point
                r = @points[0]
                @points.each do |p|
                        r = p if (p.y < r.y) or (p.y == r.y and p.x > p.y)
                end
                
                return r
        end
        
        def max_x
                mx = 0
                @points.each do |p|
                        mx = p.x if p.x > mx
                end
                
                return mx
        end
        
        def max_y
                my = 0
                @points.each do |p|
                        my = p.y if p.y > my
                end
                
                return my
        end
        
        def min_x
                mx = @points[0].x
                @points.each do |p|
                        mx = p.x if p.x < mx
                end
                
                return mx
        end
        
        def min_y
                my = @points[0].y
                @points.each do |p|
                        my = p.y if p.y < my
                end
                
                return my
        end
        
        def dist_min
                dm = map[0][0]
                
                @nb_sommet.times do |i|
                        i.times do |j|
                                dm = @map[i][j] if @map[i][j] < dm
                        end
                end
        end
        
        def center_point
                x, y = 0, 0
                
                self.points.each do |p|
                        x = x + p.x
                        y = y + p.y
                end
                x = x / self.nb_sommet
                y = y / self.nb_sommet
                
                return Point.new(x, y)
        end
        
end
        
