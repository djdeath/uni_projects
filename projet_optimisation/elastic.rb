require 'graph'
require 'timecounter'

module Elastic
        R_CYCLE = 0.5
        
        def draw_elastic(display)
                time = TimeCounter.new 
                time.start
                
                # On récupère les valeurs de paramètres
                factor_point = display.prefs['elastic_factor_point']
                k = display.prefs['elastic_k']
                k_dec = display.prefs['elastic_kdec']
                alpha = display.prefs['elastic_alpha']
                betha = display.prefs['elastic_betha']
                
                mx = self.max_x
                my = self.max_y
                
                if mx > my
                        mcoord = mx
                else
                        mcoord = my
                end
                
                dist_min = self.dist_min
                
                nb_elastic = (factor_point * self.nb_sommet).to_i
                
                elastic = []
                elastic_tmp = []
                phis = Array.new(self.nb_sommet, 0.0)
                
                nb_elastic.times do
                        elastic << Point.new(0, 0)
                        elastic_tmp << Point.new(0, 0)
                end
                
                # barycentre des points du graphe
                cpoint = self.center_point
                
                # On réparti les points de l'élastique autour
                # du barycentre des points du graphe en forme de cercle
                dangle = 2 * Math::PI / elastic.length
                angle = 0.0
                
                elastic.each do |p|
                        p.x = cpoint.x + R_CYCLE * Math.cos(angle) * mx
                        p.y = cpoint.y + R_CYCLE * Math.sin(angle) * my
                        
                        angle += dangle
                end
                
                time.stop
                nb_elastic.times do |i|
                        e1 = elastic[i]
                        e2 = elastic[i % nb_elastic]
                        
                        display.draw_edge(e1, e2)
                end
                time.start
                
                # On lance l'algorithme
                iter = 0
                loop do
                        display.set_text("itération #{iter}")
                        
                        self.nb_sommet.times do |i|
                                phis[i] = 0
                                elastic.each do |p|
                                        phis[i] += phi(p.distance(self.points[i]) / mcoord, k)
                                end
                        end
                        
                        nb_elastic.times do |i|
                                vx = 0.0
                                vy = 0.0
                                
                                dx = 0.0
                                dy = 0.0
                                
                                self.nb_sommet.times do |j|
                                        w = phi(elastic[i].distance(self.points[j]) / mcoord, k) / phis[j]
                                        
                                        vx += w * (self.points[j].x - elastic[i].x)
                                        vy += w * (self.points[j].y - elastic[i].y)
                                end
                                
                                vx *= alpha
                                vy *= alpha
                                
                                vx += betha * k * (elastic[(i - 1) % nb_elastic].x - 2 * elastic[i].x + elastic[(i + 1) % nb_elastic].x)
                                vy += betha * k * (elastic[(i - 1) % nb_elastic].y - 2 * elastic[i].y + elastic[(i + 1) % nb_elastic].y)
                                
                                elastic_tmp[i].x = elastic[i].x + vx
                                elastic_tmp[i].y = elastic[i].y + vy
                        end
                        
                        
                        eltmp = elastic
                        elastic = elastic_tmp
                        elastic_tmp = eltmp
                        
                        # Redessinage ;)
                        time.stop
                        display.clean_edges
                        nb_elastic.times do |i|
                                e1 = elastic[i]
                                e2 = elastic[(i + 1) % nb_elastic]
                                display.draw_edge(e1, e2)
                        end
                        display.redraw
                        time.start
                        
                        k *= k_dec
                        
                        iter = iter.next
                        break if nb_point_found(elastic, dist_min) >= self.nb_sommet
                end
                
                points = construct_path(elastic, dist_min)
                time.stop
                
                display.clean_edges
                w = 0.0
                points.each_index do |i|
                        n1 = points[i]
                        n2 = points[(i + 1) % self.nb_sommet]
                        p1 = self.points[n1]
                        p2 = self.points[n2]
                        w += self.map[n1][n2]
                        display.draw_edge(p1, p2)
                end
                display.redraw
                
                display.set_text("Poids final #{w} en #{iter} itérations")
                display.stats.set_stat("Élastique", time.to_s, w)
        end
        
        #######
        private
        #######
        
        def phi(d, k)
                return Math.exp(- (d * d) / (2 * k * k))
        end
        
        # Cette méthode compte le nombre de point de
        # l'élastique qui sont suffisament proche d'un
        # point du graphe pour être considéré comme trouvé.
        def nb_point_found(elastic, dist_min)
                found = 0
                
                self.nb_sommet.times do |i|
                        elastic.length.times do |j|
                                if self.points[i].distance(elastic[j]) < dist_min
                                        found = found.next
                                        break
                                end
                        end
                end
                
                return found
        end
        
        # Cette méthode permet de contruire le chemin à partir
        # de l'élastique. Comme l'on peut s'en rendre compte sur
        # certaines configuration, le chemin représenté par
        # l'élastique ne passe pas tjrs par chaque point une fois
        # et une seule. Il faut donc construire le chemin du cycle
        # en faisant attention à ne par repasser par un sommet déjà
        # visité.
        def construct_path(elastic, dist_min)
                points = []
                
                elastic.each do |p|
                        self.points.each_with_index do |gp, i|
                                if p.distance(gp) < dist_min and not points.index(i)
                                        points << i
                                        break
                                end
                        end
                end
                
                return points
        end
        
end
