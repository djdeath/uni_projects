# Ce module implémente l'algorithme de Prim ainsi
# que l'heuristique se basant sur l'arbre recouvant.

require 'heap'
require 'edge'
require 'graph'
require 'tree'

require 'display'

module Prim
        
        def draw_tree(display)
                
                edges = Array.new(self.nb_sommet, nil)
                heap = Heap.new(self.nb_sommet * self.nb_sommet)
                
                nb_arcs = 0
                tree_weight = 0
                
                # On empile une première arête bidon
                heap.put(Edge.new(0, 0, 0.0))
                
                while ((nb_arcs < self.nb_sommet - 1) and !heap.empty?)
                        
                        # On sort une arête du tas
                        ed = heap.pop
                        
                        if (edges[ed.top] == nil)
                                cur = ed.top
                        elsif (edges[ed.prev] == nil)
                                cur = ed.prev
                        else
                                next
                        end
                        
                        edges[cur] = ed
                        tree_weight += ed.weight
                        
                        # Mise à jour de l'interface graphique
                        if (ed.top != ed.prev)
                                display.set_text("Current weight : #{tree_weight}")
                                display.draw_edge_ids(ed.top, ed.prev)
                        end
                        
                        # Redessinage de l'interface graphique
                        while (Gtk.events_pending?)
                                Gtk.main_iteration
                        end
                        
                        # Ajout des nouveaux sommets
                        for i in 0..(self.nb_sommet - 1)
                                if (edges[i] == nil)
                                        heap.put(Edge.new(i, cur, self.map[i][cur]))
                                end
                        end
                        
                end
                
        end
        
        def covering_tree
                
                # Arbre et tableau permettant l'accès direct aux noeuds
                tree = Tree.new(0)
                noeuds = Array.new(self.nb_sommet)
                noeuds[0] = tree
                
                edges = Array.new(self.nb_sommet, nil)
                heap = Heap.new(self.nb_sommet * self.nb_sommet)
                
                nb_arcs = 0
                tree_weight = 0
                
                # On empile une première arête bidon
                heap.put(Edge.new(0, 0, 0.0))
                
                while ((nb_arcs < self.nb_sommet - 1) and !heap.empty?)
                        
                        # On sort une arête du tas
                        ed = heap.pop
                        
                        if (edges[ed.top] == nil)
                                cur = ed.top
                        elsif (edges[ed.prev] == nil)
                                cur = ed.prev
                        else
                                next
                        end
                        
                        edges[cur] = ed
                        tree_weight += ed.weight
                        
                        # Ajout des nouveaux sommets
                        for i in 0..(self.nb_sommet - 1)
                                if (edges[i] == nil)
                                        heap.put(Edge.new(i, cur, self.map[i][cur]))
                                end
                        end
                        
                        
                        if (ed.top != ed.prev)
                                # Mise à jour de l'arbre
                                noeuds[ed.top] = Node.new(ed.top)
                                noeuds[ed.prev].append(noeuds[ed.top])
                        end
                end
                
                return tree
        end
        
        def draw_route(display)
                time = TimeCounter.new
                time.start
                
                route_weight = 0
                tree = covering_tree
                list = tree.prefix_list
                
                list.each_index do |i|
                        
                        # 2 points
                        j = list[i]
                        k = list [(i + 1).modulo(self.nb_sommet)]
                        route_weight += self.map[j][k]
                        
                        time.stop
                        display.set_text("Current weight : #{route_weight}")
                        display.draw_edge_ids(j, k)
                        #display.queue_draw
                        
                        # Redessinage de l'interface graphique
                        display.redraw
                        time.start
                end
                
                display.stats.set_stat('Covering tree', time.to_s, route_weight)
        end

end
