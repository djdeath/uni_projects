class Node
        attr_accessor :data, :children
        
        def initialize(data)
                @children = []
                @data = data
        end
        
        def append(node)
                @children << node
        end
        
        def delete(node)
                @children.delete(node)
        end
        
end

class Tree < Node
        
        def initilize(data)
                super(data)
        end
        
        private
        
        def prefix_list_(list, node)
                list << node.data
                
                node.children.each do |n|
                        prefix_list_(list, n)
                end
                
                return list
        end
        
        public
        
        # Renvoie un tableau contenant les données
        # de chaque node triées par ordre prefix
        def prefix_list
                return prefix_list_([], self)
        end
        
end
