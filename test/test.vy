(setvar 'nil 'nil)
(setvar 'number? 
    (fn (x)
        (or (int? x) (float? x))))

(setvar 'average-2 
    (fn (a b)
        (if (and (number? a) (number? b))
           (/ (+ a b) 2)
           nil)))

(average-2 4 8)
(average-2 3 'Hello)
(average-2 'Goodbye "here")

