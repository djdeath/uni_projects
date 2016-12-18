class TimeCounter
        
        def initialize
                @spent = 0
        end
        
        def start
                @stime = Time.new
        end
        
        def stop
                return if @stime == nil
                htime = Time.new
                @spent += htime - @stime
                puts "spent time = #{@spent}"
        end
        
        def to_s
                spent = @spent.to_i
                
                hours = spent / 3600
                minutes = (spent - hours * 3600) / 60
                seconds = spent - hours * 3600 - minutes * 60
                
                if hours == 0
                        return "#{seconds}seconde(s)" if minutes == 0
                        return "#{minutes}minute(s) #{seconds}seconde(s)"
                end
                
                return "#{hours}heure(s) #{minutes}minute(s) #{seconds}seconde(s)"
        end
        
end
