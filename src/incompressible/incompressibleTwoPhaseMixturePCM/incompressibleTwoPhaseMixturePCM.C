/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "incompressibleTwoPhaseMixturePCM.H"
#include "addToRunTimeSelectionTable.H"
#include "surfaceFields.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(incompressibleTwoPhaseMixture, 0);
}


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

void Foam::incompressibleTwoPhaseMixture::calcNu()
{
    nuModel1_->correct();
    nuModel2_->correct();

    const volScalarField limitedAlpha1
    (
        "limitedAlpha1",
        min(max(alpha1_, scalar(0)), scalar(1))
    );

    // Average kinematic viscosity calculated from dynamic viscosity

    nu_ = mu()/(limitedAlpha1*rho1_ + (scalar(1) - limitedAlpha1)*rho2_);

    k_  = (limitedAlpha1*k1_ + (scalar(1) - limitedAlpha1)*k2_);
    
        
    const volScalarField::Boundary& limitedAlpha1bf = limitedAlpha1.boundaryField();
    const volScalarField::Boundary& alpha1bf = alpha1_.boundaryField();
    volScalarField::Boundary& kbf = k_.boundaryFieldRef();

    forAll(kbf, patchi)
    {
        const fvPatchScalarField& alpha1p = alpha1bf[patchi];
        const fvPatchScalarField& limitedAlpha1p = limitedAlpha1bf[patchi];
        fvPatchScalarField& kp = kbf[patchi];
        
        forAll(kp, facei)
        {
            kp[facei]=(limitedAlpha1p[facei]*k1_.value() + (scalar(1) - limitedAlpha1p[facei])*k2_.value());
            
            Info<<"kp["<<facei<<"]="<<kp[facei]<<"    limitedAlpha1p="<<limitedAlpha1p[facei]<<"    alpha1p="<<alpha1p[facei]<<endl;
        }
        
    }

    L_ = (limitedAlpha1*L1_ + (scalar(1) - limitedAlpha1)*L2_);

    {
        const volScalarField& T = nu_.mesh().lookupObject<volScalarField>("T");

        const dimensionedScalar deltaT2=(T2_-T1_)*(T2_-T1_);
        const dimensionedScalar Tm=(T2_+T1_)/2;
 
        const dimensionedScalar dnom=Foam::sqrt(deltaT2*Foam::acos(-1.));

        volScalarField DL=Foam::exp(
                                      -Foam::pow( (T-Tm+dimensionedScalar("SMALL",T.dimensions(),SMALL)),2)
                                      /deltaT2 
                                    )
                                    /(dnom)*L_;//In the second ref Eq. 9: \landa D        
        Cp_ = 
                (limitedAlpha1*(rho1_*cp1_) + (scalar(1) - limitedAlpha1)*(rho2_*cp2_));
                (limitedAlpha1* rho1_       + (scalar(1) - limitedAlpha1)*rho2_       )*DL;//Discuss this one

    }
    
    
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::incompressibleTwoPhaseMixture::incompressibleTwoPhaseMixture
(
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    IOdictionary
    (
        IOobject
        (
            "transportProperties",
            U.time().constant(),
            U.db(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    twoPhaseMixture(U.mesh(), *this),

    nuModel1_
    (
        viscosityModelPCM::New
        (
            "nu1",
            subDict(phase1Name_),
            U,
            phi
        )
    ),
    nuModel2_
    (
        viscosityModelPCM::New
        (
            "nu2",
            subDict(phase2Name_),
            U,
            phi
        )
    ),

    rho1_("rho", dimDensity, nuModel1_->viscosityProperties()),
    rho2_("rho", dimDensity, nuModel2_->viscosityProperties()),
    cp1_ ("cp" , dimEnergy/dimMass/dimTemperature, nuModel1_->viscosityProperties()),
    cp2_ ("cp" , dimEnergy/dimMass/dimTemperature, nuModel2_->viscosityProperties()),
    k1_  ("k"  , dimPower/dimLength/dimTemperature, nuModel1_->viscosityProperties()),
    k2_  ("k"  , dimPower/dimLength/dimTemperature, nuModel2_->viscosityProperties()),
    L1_  ("L"  , dimEnergy/dimMass, nuModel1_->viscosityProperties()),
    L2_  ("L"  , dimEnergy/dimMass, nuModel2_->viscosityProperties()),
    T1_  ("T"  , dimTemperature, nuModel1_->viscosityProperties()),
    T2_  ("T"  , dimTemperature, nuModel2_->viscosityProperties()),
    U_(U),
    phi_(phi),

    nu_
    (
        IOobject
        (
            "nu",
            U_.time().timeName(),
            U_.db()
        ),
        U_.mesh(),
        dimensionedScalar(dimViscosity, 0),
        calculatedFvPatchScalarField::typeName
    ),
    Cp_
    (
        IOobject
        (
            "Cp",
            U_.time().timeName(),
            U_.db(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        U_.mesh(),
        dimensionedScalar(dimEnergy/dimMass/dimTemperature*dimDensity, 0),
        calculatedFvPatchScalarField::typeName
    ),
    k_
    (
        IOobject
        (
            "kappa",
            U_.time().timeName(),
            U_.db(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        U_.mesh(),
        dimensionedScalar(dimPower/dimLength/dimTemperature, 0),
        calculatedFvPatchScalarField::typeName
    ),
    L_
    (
        IOobject
        (
            "L",
            U_.time().timeName(),
            U_.db()
        ),
        U_.mesh(),
        dimensionedScalar(dimEnergy/dimMass, 0),
        calculatedFvPatchScalarField::typeName
    )
{
    calcNu();

    Info<<"incompressibleTwoPhaseMixture done"<<endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::incompressibleTwoPhaseMixture::mu() const
{
    const volScalarField limitedAlpha1
    (
        min(max(alpha1_, scalar(0)), scalar(1))
    );

    return volScalarField::New
    (
        "mu",
        limitedAlpha1*rho1_*nuModel1_->nu()
      + (scalar(1) - limitedAlpha1)*rho2_*nuModel2_->nu()
    );
}


Foam::tmp<Foam::surfaceScalarField>
Foam::incompressibleTwoPhaseMixture::muf() const
{
    const surfaceScalarField alpha1f
    (
        min(max(fvc::interpolate(alpha1_), scalar(0)), scalar(1))
    );

    return surfaceScalarField::New
    (
        "muf",
        alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
      + (scalar(1) - alpha1f)*rho2_*fvc::interpolate(nuModel2_->nu())
    );
}


Foam::tmp<Foam::surfaceScalarField>
Foam::incompressibleTwoPhaseMixture::nuf() const
{
    const surfaceScalarField alpha1f
    (
        min(max(fvc::interpolate(alpha1_), scalar(0)), scalar(1))
    );

    return surfaceScalarField::New
    (
        "nuf",
        (
            alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
          + (scalar(1) - alpha1f)*rho2_*fvc::interpolate(nuModel2_->nu())
        )/(alpha1f*rho1_ + (scalar(1) - alpha1f)*rho2_)
    );
}


bool Foam::incompressibleTwoPhaseMixture::read()
{
    if (regIOobject::read())
    {
        if
        (
            nuModel1_().read
            (
                subDict(phase1Name_ == "1" ? "phase1": phase1Name_)
            )
         && nuModel2_().read
            (
                subDict(phase2Name_ == "2" ? "phase2": phase2Name_)
            )
        )
        {
            nuModel1_->viscosityProperties().lookup("rho") >> rho1_;
            nuModel2_->viscosityProperties().lookup("rho") >> rho2_;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
