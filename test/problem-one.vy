(def average-2 (x y)
    (if [(number? x) and (number? y)]
        [[x + y] / 2]
        nil))

