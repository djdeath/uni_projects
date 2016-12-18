# Cette classe implémente l'algorithme du tas.
# Les éléments qu'elle manipule doivent implémenter l'opérateur <=>

class Heap
        
        def initialize(size)
                @heap = Array.new(size)
                @size = size
                @nb = 0
        end
        
        # Calcul des parents/enfants
        def parent(node)
                return (node - 1) / 2
        end
        
        def left_child(node)
                return 2 * node + 1
        end
        
        def right_child(node)
                return 2 * node + 2
        end
        
        # Ajouter un element au tas
        def put(data)
                
                if (@nb >= @size)
                        raise "Heap full"
                end
                
                p = @nb
                pp = parent(p)
                
                while ((p != 0) && ((data <=> @heap[pp]) < 0))
                        @heap[p] = @heap[pp]
                        
                        p = pp
                        pp = parent(p)
                end
                
                @heap[p] = data
                @nb = @nb + 1
                
        end
        
        
        # Retirer un élément au tas
        def pop
                
                if (@nb <= 0)
                        raise "Heap empty"
                end
                
                @nb = @nb - 1
                
                ret = @heap[0]
                v = @heap[@nb]
                
                p = 0
                
                while (p < @nb)
                        l = left_child(p)
                        r = right_child(p)
                        
                        if (l > @nb)
                                break
                        else
                                if (r > @nb)
                                        n = l
                                else
                                        if (@heap[l] <=> @heap[r]) < 0
                                                n = l
                                        else
                                                n = r
                                        end
                                end
                        end
                        
                        if (v <=> @heap[n]) < 0
                                break
                        end
                        
                        @heap[p] = @heap[n]
                        
                        p = n
                end
                @heap[p] = v
                
                return ret
        end
        
        # Le tas est il vide ?
        def empty?
                return (@nb <= 0)
        end
        
end
