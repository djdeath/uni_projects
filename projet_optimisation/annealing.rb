require 'graph'
require 'timecounter'

require 'display'

module Annealing
        
        def draw_annealing(display)
                # Compte le temps écoulé
                time = TimeCounter.new
                time.start
                
                # Température
                t0 = display.prefs['annealing_t0']
                t = t0
                mut = display.prefs['annealing_tdec']
                mu = mut
                
                # Remplissage du tableau des arêtes avec une
                # méthode quelque peu... hmmm... simplette ;)
                nexts = Array.new(self.nb_sommet)
                prevs = Array.new(self.nb_sommet)
                n = 1
                p = self.nb_sommet - 1
                nexts.each_index do |i|
                        nexts[i] = n % self.nb_sommet
                        prevs[i] = p % self.nb_sommet
                        n = n.next
                        p = p.next
                end
                
                time.stop # arrêt du temps pour dessiner
                # On dessine le tout
                weight = 0
                nexts.each_with_index do |p1, p2|
                        weight += self.map[p1][p2]
                        display.draw_edge_ids(p1, p2)
                end
                display.set_text("Poids courant : #{weight}")
                time.start
                
                n = 0 # nombre de transformation
                nb = self.nb_sommet * (self.nb_sommet - 1)
                while n < nb
                        
                        # On tire au hasard 2 arêtes
                        a1 = Kernel::rand(self.nb_sommet)
                        a2 = Kernel::rand(self.nb_sommet)
                        while (a2 == a1) or (nexts[a1] == a2) or (prevs[a1] == a2)
                                a2 = Kernel::rand(self.nb_sommet)
                        end
                        
                        a3 = nexts[a1]
                        a4 = nexts[a2]
                                
                        # On calcule le coût de ces arêtes
                        oc1 = self.map[a1][a3]
                        oc2 = self.map[a2][a4]
                        
                        # On calcule le coût des nouvelles arêtes
                        nc1 = self.map[a1][a2]
                        nc2 = self.map[a3][a4]
                        
                        # Le nouveau cycle est il moins coûteux ?
                        if (nc1 + nc2) < (oc1 + oc2)
                                opt2(nexts, prevs, a1, a2)
                                
                                # On fait décroitre t
                                t = mut * t
                                #mut *= mu
                                
                                n = 0
                        else
                                q = 10000000
                                p = Kernel::rand(q) / q
                                if p < Math::exp(-Math::sqrt(self.nb_sommet) * (nc1 + nc2) - (oc1 + oc2) / t)
                                        opt2(nexts, prevs, a1, a2)
                                        n = 0
                                        t = mut * t
                                else        
                                        n = n.next
                                end
                        end
                       
                        time.stop # arrêt du temps pour redessiner
                        if n == 0 # Affichage si modification
                                display.clean_edges
                                weight = 0
                                nexts.each_with_index do |p1, p2|
                                        weight += self.map[p1][p2]
                                        display.draw_edge_ids(p1, p2)
                                end
                                display.set_text("Poids courant : #{weight}")
                                display.redraw
                        end
                        time.start
                end
                
                time.stop
                
                display.set_text("Poids final : #{weight}")
                display.stats.set_stat('Récuit simulé', time.to_s, weight)
        end
        
        #######
        private
        #######
        
        # 2 échange
        def opt2(nexts, prevs, a1, a2)
                a3 = nexts[a1]
                a4 = nexts[a2]
                
                # On procède à l'échange des arÃÂªtes
                nexts[a1] = a2
                nexts[a3] = a4
                prevs[a4] = a3
                
                # Mise à jour des circuits nexts et prevs
                p = a1
                c = a2
                while (c != a3)
                        nexts[c] = prevs[c]
                        prevs[c] = p
                        p = c
                        c = nexts[p]
                end
                prevs[a3] = p
        end
        
end
