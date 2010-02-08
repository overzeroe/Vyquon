; Set necessary symbols
(setvar 'nil 'nil)
(setvar 'false 'false)
(setvar 'true 'true)

(setvar 'test 
    (fn (a ? x .. y) (list a x y)))
(test 5 100 20 30 40 50)
