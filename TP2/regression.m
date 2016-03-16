function [ c, r2 ] = regression( X, Y )
    %From http://www.mathworks.com/help/matlab/data_analysis/linear-regression.html
    c = polyfit(X,Y,1);
    yFit = polyval(c, X);
    yResid = Y - yFit;
    SSResid = sum(yResid.^2);
    SSTotal = (length(Y)-1) * var(Y);
    r2 = 1 - SSResid / SSTotal;
end

