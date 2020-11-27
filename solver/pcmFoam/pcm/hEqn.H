solve
(
    fvm::ddt(Cp,T)+fvm::div(fvc::interpolate(Cp),T)==fvc::laplacian(k,T)
);
