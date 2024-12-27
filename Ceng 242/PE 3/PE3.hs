module PE3 where

data Expression = Variable String 
    | Constant Float
    | Sine Expression
    | Cosine Expression
    | Negation Expression
    | Addition Expression Expression
    | Multiplication Expression Expression

class Differential a where
    diff :: a -> a -> a

instance Show Expression where
    show _ = ""

instance Eq Expression where
    (==) _ _ = False

instance Num Expression where
    fromInteger _ = Constant 0
    (*) _ _ = Constant 0
    (+) _ _ = Constant 0
    negate _ = Constant 0


instance Differential Expression where
    diff _ _ = Constant 0

shuntingyard :: [String] -> [String] -> [String] -> [String]
shuntingyard _ _ _ = []
