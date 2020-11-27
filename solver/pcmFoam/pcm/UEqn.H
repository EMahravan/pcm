    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U) + fvm::div(rhoPhi, U)
      + turbulence->divDevRhoReff(rho, U)
     ==
        fvOptions(rho, U)
    );

    UEqn.relax();

    fvOptions.constrain(UEqn);

    const dimensionedScalar Amush("Amush",dimArea,100.);//In the second ref: 1e5. 
     
    tmp<volVectorField> F
    (
    new volVectorField
        (
            IOobject
            (
                "F",
                runTime.timeName(),
                mesh,
                IOobject::READ_IF_PRESENT
            ),
            Foam::sqr(1.-alpha1)/(Foam::pow(alpha1,3)+0.001)*Amush*U//In the second Ref it's negetive
        )
    );

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(p_rgh)
                  
                ) * mesh.magSf()
            )+ F
        );

        fvOptions.correct(U);
    }
