{
    if ($1 == "Bandwidth" && $5 == "threads" && $8 == "ceil") {
        bandwidth = $3
        threads = substr($7, 1, length($7) - 1)
        ceil = substr($10, 1, length($10) - 1)
        avg = 0
        
        if (threads == 1 && ceil == 65) { # For each bandwidth
            printf("%2d Mbps\n", bandwidth / 1000) 
            print("==================================================================================")
            print("Ceil Rate  |  65%  70%  75%  80%  85%  90%  95% 100% |   40ms   60ms   80ms  100ms")
            print("-----------+-----------------------------------------+----------------------------")
        }
        if (ceil == 65) { # For each number of threads
            printf("%2d Threads |", threads);
        }
    }
    
    if ($1 >= 1 && $1 <= 32 && $2 == "|") # Calc sum
    {
        avg += $7
    }
    
    if ($1 == "32" && $2 == "|")
    {
        avg = avg / 32 # Calc average
        printf("%5d", avg + 0.5) # Output
        avgs[ceil] = avg # Store the Delay (for Linear Interpolation Lator)
        if (ceil == 100) {
            printf(" |")
            for (target = 40; target <= 100; target += 20) { # Linear Interpolation
                if (target < avgs[65])
                    result = 65
                else if (target < avgs[70])
                    result = (65 * (avgs[70] - target) + 70 * (target - avgs[65])) / (avgs[70] - avgs[65])
                else if (target < avgs[75])
                    result = (70 * (avgs[75] - target) + 75 * (target - avgs[70])) / (avgs[75] - avgs[70])
                else if (target < avgs[80])
                    result = (75 * (avgs[80] - target) + 80 * (target - avgs[75])) / (avgs[80] - avgs[75])
                else if (target < avgs[85])
                    result = (80 * (avgs[85] - target) + 85 * (target - avgs[80])) / (avgs[85] - avgs[80])
                else if (target < avgs[90])
                    result = (85 * (avgs[90] - target) + 90 * (target - avgs[85])) / (avgs[90] - avgs[85])
                else if (target < avgs[95])
                    result = (90 * (avgs[95] - target) + 95 * (target - avgs[90])) / (avgs[95] - avgs[90])
                else if (target < avgs[100]) {
                    result = (95 * (avgs[100] - target) + 100 * (target - avgs[95])) / (avgs[100] - avgs[95])
                }
                else
                    result = 100
                
                printf("%6.1f%%", result) # Print Result
            }
            
            printf("\n") # Print Table
            if (threads == 15)
                print("==================================================================================")
        }
    }
}

