package main

import (
    "fmt"
    "runtime"
    "time"
)

func main () {
    fmt.Println("Go runs on")
    switch os :=runtime.GOOS; os {
        case "Linux":
            fmt.Println("Linux.")
        case "Ubuntu":
            fmt.Println("Ubuntu.")
        default:
            fmt.Printf("%s.\n", os)

    }

    today := time.Now().Weekday()
    switch time.Saturday {
        case today +0:
            fmt.Println("Today");
        case today +1:
            fmt.Println("Tomorrow");
        case today +2:
            fmt.Println("In two days");
        default:
            fmt.Println("Too far way");
    }

    t := time.Now()
    fmt.Println("t:", t.Hour())
    switch {
        case t.Hour() < 12:
            fmt.Println("Good morning")
        case t.Hour() < 17:
            fmt.Println("Good afternoon")
        default:
            fmt.Println("Good evening")
    }

}
