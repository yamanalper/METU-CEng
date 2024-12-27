module PE1 where

-- Question 1
findInList :: (Eq a) => [a] -> [a] -> [Int]
findInList _ [] = []
findInList [] _ = []
findInList (x:rx) (y:ry) = findInList' (x:rx) (y:ry)
	where
		firstList = (y:ry)
		findInList' _ [] = []
		findInList' [] _ = []
		findInList' (x:rx) (y:ry)
						  | x == y    = [ ( length firstList - length (y:ry) ) + (index y (y:ry)) ] ++ findInList' rx (y:ry)
					   	  | otherwise = findInList' (x:rx) ry


index _ [] = -1
index k (v:rest)    | k == v   = 0
					| otherwise = let inrest = index k rest
								in if inrest == -1 then -1
									else 1 + inrest




-- Question 2
--findInMatrix :: (Eq a) => [[a]] -> [[a]] -> [[(Int, Int)]]
findInMatrix _ [] = []
findInMatrix [] _ = []
findInMatrix x y = helper pattern search
	where
		pattern = findInList (concat x) (concat (matrixTraverse y))
		search = concat (matrixTraverseIndices y)
		


helper _ [] = []
helper [] _ = []
helper (l:rest) list = [list!!l] ++ helper rest list



matrixTraverse [] = repeat []
matrixTraverse (s:rs) = takeWhile (not . null) $
    zipWith (++) (map (:[]) s ++ repeat [])
                 ([]:matrixTraverse rs)
matrixTraverseIndices [] = repeat []
matrixTraverseIndices matrix = transpose [ [ (i, j) | (j, _) <- zip [0..] row ] | (i, row) <- zip [0..] matrix ]

transpose ([]:_) = []
transpose x = (map head x) : transpose (map tail x)