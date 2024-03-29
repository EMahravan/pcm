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

#include "CrossPowerLaw.H"
#include "addToRunTimeSelectionTable.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace viscosityModelPCMs
{
    defineTypeNameAndDebug(CrossPowerLaw, 0);

    addToRunTimeSelectionTable
    (
        viscosityModelPCM,
        CrossPowerLaw,
        dictionary
    );
}
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::viscosityModelPCMs::CrossPowerLaw::calcNu() const
{
    return (nu0_ - nuInf_)/(scalar(1) + pow(m_*strainRate(), n_)) + nuInf_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::viscosityModelPCMs::CrossPowerLaw::CrossPowerLaw
(
    const word& name,
    const dictionary& viscosityProperties,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    viscosityModelPCM(name, viscosityProperties, U, phi),
    CrossPowerLawCoeffs_
    (
        viscosityProperties.optionalSubDict(typeName + "Coeffs")
    ),
    nu0_("nu0", dimViscosity, CrossPowerLawCoeffs_),
    nuInf_("nuInf", dimViscosity, CrossPowerLawCoeffs_),
    m_("m", dimTime, CrossPowerLawCoeffs_),
    n_("n", dimless, CrossPowerLawCoeffs_),
    nu_
    (
        IOobject
        (
            name,
            U_.time().timeName(),
            U_.db(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        calcNu()
    )
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::viscosityModelPCMs::CrossPowerLaw::read
(
    const dictionary& viscosityProperties
)
{
    viscosityModelPCM::read(viscosityProperties);

    CrossPowerLawCoeffs_ =
        viscosityProperties.optionalSubDict(typeName + "Coeffs");

    CrossPowerLawCoeffs_.lookup("nu0") >> nu0_;
    CrossPowerLawCoeffs_.lookup("nuInf") >> nuInf_;
    CrossPowerLawCoeffs_.lookup("m") >> m_;
    CrossPowerLawCoeffs_.lookup("n") >> n_;

    return true;
}


// ************************************************************************* //
