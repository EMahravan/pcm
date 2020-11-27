/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2014-2018 OpenFOAM Foundation
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

Class
    Foam::immiscibleIncompressibleTwoPhaseMixturePCM

Description
    An immiscible incompressible two-phase mixture transport model

SourceFiles
    immiscibleIncompressibleTwoPhaseMixturePCM.C

\*---------------------------------------------------------------------------*/

#ifndef immiscibleIncompressibleTwoPhaseMixturePCM_H
#define immiscibleIncompressibleTwoPhaseMixturePCM_H

#include "incompressibleTwoPhaseMixture.H"
#include "interfaceProperties.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
           Class immiscibleIncompressibleTwoPhaseMixturePCM Declaration
\*---------------------------------------------------------------------------*/

class immiscibleIncompressibleTwoPhaseMixturePCM
:
    public incompressibleTwoPhaseMixture,
    public interfaceProperties
{

public:

    // Constructors

        //- Construct from components
        immiscibleIncompressibleTwoPhaseMixturePCM
        (
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    virtual ~immiscibleIncompressibleTwoPhaseMixturePCM()
    {}


    // Member Functions

        //- Correct the transport and interface properties
        virtual void correct()
        {
            incompressibleTwoPhaseMixture::correct();
            interfaceProperties::correct();
        }

        //- Read base transportProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
