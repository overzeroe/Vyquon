(setvar 'nil 'nil)
(setvar 'false 'false)
(setvar 'true 'true)

(setvar 'number? 
    (fn (x)
        (or (int? x) (float? x))))

(setvar 'average-2 
    (fn (a b)
        (if (and (number? a) (number? b))
           (/ (+ a b) 2)
           nil)))

(average-2 100 200)

