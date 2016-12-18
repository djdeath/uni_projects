# Ce module implémente l'heuristique d'insertion du sommet le plus éloigné

require 'edge'
require 'graph'

require 'display'

module Far
        
        def draw_far(display)
                
                time = TimeCounter.new
                time.start
                
                # Initialisation :
                # tableau des arrêtes minimum
                dist_min = Array.new(self.nb_sommet)
                self.map[0].each_index do |i|
                        dist_min[i] = self.map[0][i]
                end
                
                # tableau des sommets correspondants
                sommet_min = Array.new(self.nb_sommet, 0)
                
                # arêtes à l'instant t
                edges = Array.new(self.nb_sommet)
                edges[0] = Edge.new(0, 0, 0.0)
                
                cycle_weight = 0.0
                
                # Algo :
                self.nb_sommet.times do
                        
                        # trouver le minimum dans le tableau dist_min
                        min = 0
                        dist_min.each_index do |i|
                                if edges[i] == nil
                                        if (dist_min[i] > dist_min[min] or edges[min] != nil)
                                                min = i
                                        end
                                end
                        end
                        
                        # On fabrique les 2 arêtes manquantes pour
                        # inserer le nouveau sommet
                        j = sommet_min[min]
                        old_ed = edges[j]
                        ed1 = Edge.new(min, j, self.map[min][j])
                        ed2 = Edge.new(old_ed.top, min, self.map[old_ed.top][min])
                        
                        # On met à jour les arêtes du cycles
                        edges[j] = ed1
                        edges[min] = ed2
                        
                        # On met à jour les tableaux dist_min et sommet_min
                        self.map[min].each_index do |i|
                                if self.map[min][i] < dist_min[i]
                                        dist_min[i] = self.map[min][i]
                                        sommet_min[i] = min
                                end
                        end
                        
                        # Enfin on redessine
                        display.clean_edges
                        
                        time.stop
                        cycle_weight = 0.0
                        edges.each do |e|
                                if e != nil
                                        cycle_weight += e.weight
                                        display.draw_edge_ids(e.top, e.prev)
                                end
                        end
                        display.set_text("Poid courant : #{cycle_weight}")
                        
                        display.redraw
                        time.start
                        
                end
                time.stop
                
                display.set_text("Poid final : #{cycle_weight}")
                display.stats.set_stat('Insertion du plus éloigné', time.to_s, cycle_weight)
        end
        
end
