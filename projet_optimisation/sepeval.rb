# Ce implémente l'algorithme de séparation/évaluation qui donne une
# solution exacte du problème du voyageur de commerce avec une
# complexité exponentielle.

require 'graph'

require 'display'

module Sepeval
        
        def draw_sepval(display)
                @time = TimeCounter.new
                @time.start
                
                @used_tops = Array.new(self.nb_sommet, -1)
                @tmp_solution = Array.new(self.nb_sommet, -1)
                @solution = Array.new(self.nb_sommet, -1)
                
                weight = draw_sepval_tab(display, self.nb_sommet, 0, -1, 0)
                @time.stop
                
                @solution.each_index do |i|
                        display.draw_edge_ids(i, @solution[i])
                end
                
                display.set_text("Poids final : #{weight}")
                display.stats.set_stat('Séperation évalution', @time.to_s, weight)
        end
        
        #######
        private
        #######
        
        def compute_min(line)
                min = -1
                self.map[line].each_index do |i|
                        if (i != line and @used_tops[i] == -1) or i == 0
                                if min == -1 or self.map[line][i] < self.map[line][min]
                                        min = i
                                end
                        end
                end
                
                return min
        end
        
        # Vérifie la validitée d'une solution
        def compute_valid(nb, top)
                visited = Array.new(self.nb_sommet, 0)
                
                p = top
                nb.times do |i|
                        if visited[p] > 0
                                return false
                        end
                        visited[p] = 1
                        p = @tmp_solution[p]
                end
                
                # Si l'on ne revient pas sur le sommet de départ
                if p != 0
                        return false
                end
                
                return true
        end
        
        def draw_sepval_tab(display, nb, current, weight, pweight)
                
                # calcul de la borne inférieure
                min = pweight
                self.nb_sommet.times do |i|
                        if @used_tops[i] == -1
                                m  = compute_min(i) # On retient les arêtes
                                @tmp_solution[i] = m
                                min = min + self.map[i][m]
                        else
                                @tmp_solution[i] = @used_tops[i]
                        end
                end
                
                display.redraw
                
                # On ne peut pas trouver de meilleure solution dans cette branche : cassos ! 
                if min > weight and weight != -1
                        return weight
                end
                
                
                # vérification de la validitée de la solution
                if compute_valid(nb, current) == true
                        # Recopie de la solution valide
                        @tmp_solution.each_index do |i|
                                @solution[i] = @tmp_solution[i]
                        end
                        
                        @time.stop
                        display.clean_edges
                        @tmp_solution.each_index do |i|
                                display.draw_edge_ids(i, @tmp_solution[i])
                        end
                        display.set_text("Poids courant : #{min}")
                        #display.queue_draw
                        display.redraw
                        @time.start
                        
                        return min
                end
                
                
                # Si la solution n'est pas valide on partitionne
                @used_tops[current] = 0
                @used_tops.each_index do |i|
                        if @used_tops[i] == -1
                                @used_tops[current] = i
                                
                                # Si on trouve une meilleur solution on mémorise son poids
                                w = draw_sepval_tab(display, nb - 1, i, weight,
                                                    pweight + self.map[current][i])
                                if weight == -1 or w < weight
                                        weight = w
                                end
                        end
                end
                @used_tops[current] = -1
                
                return weight
        end
end
