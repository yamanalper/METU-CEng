module PE2 where

data Tree k v = EmptyTree | Node k v [Tree k v] deriving (Show, Eq)

-- Question 1
selectiveMap :: (a -> Bool) -> (a -> a) -> [a] -> [a]
selectiveMap _ _ _ = []

-- Question 2
tSelectiveMap :: (k -> Bool) -> (v -> v) -> Tree k v -> Tree k v
tSelectiveMap _ _ _ = EmptyTree

-- Question 3
tSelectiveMappingFold :: (k -> Bool) -> (k -> v -> r) -> (r -> r -> r) -> r -> Tree k v -> r
tSelectiveMappingFold _ _ _ idt _ = idt

-- Question 4
-- This question commented out so that your file compiles even before you complete it
-- It shouldn't effect the grades of other questions when commented out
-- When ready, just remove the lines with {- and -} to uncomment the function
{-
searchTree :: (Eq v, Eq k) => v -> Tree k v -> (k -> v)
searchTree def = tSelectiveMappingFold a b c d
    where a = 
          b = 
          c = 
          d = 
-}
