function val = tif(condition, a, b)
% TIF  Ternary if, analogous to the C ternary operator ?: .

% Author: Paolo D'Apice

if condition
    val = a;
else
    val = b;
end