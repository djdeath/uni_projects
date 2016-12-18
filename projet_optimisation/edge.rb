# Cette classe représente les arrêtes des graphes

require 'point'

class Edge
        attr_reader :top, :prev, :weight
        
        include Comparable
        
        def initialize(top, prev, weight)
                @top = top
                @prev = prev
                @weight = weight
        end
        
        def <=>(e)
                if (@weight == e.weight)
                        if (@top == e.top)
                                
                                if (@prev > e.prev)
                                        return 1
                                else
                                        return -1
                                end
                                
                        else
                                
                                if (@top > e.top)
                                        return 1
                                else
                                        return -1
                                end
                                
                        end
                end
                
                if (@weight > e.weight)
                        return 1
                else
                        return -1
                end
                
        end
        
        # Utile pour le débugage
        def to_s
                top.to_s + " -> " + prev.to_s + "(" + weight.to_s + ")"
        end
        
end
