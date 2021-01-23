surfaceScalarField phiCp
(
    IOobject
    (
        "phiCp",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT
    ),
    (fvc::interpolate(Cp)*phi)
);

solve
(
    fvm::ddt(Cp,T)+fvm::div(phiCp,T)==fvc::laplacian(k,T)
);


