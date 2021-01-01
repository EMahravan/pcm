
solve
(
    fvm::ddt(Cp,T)+Cp*fvm::div(phi,T)==fvc::laplacian(k,T)
);


