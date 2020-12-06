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

Class
    Foam::incompressibleTwoPhaseMixturePCM

Description
    A two-phase incompressible transportModel

SourceFiles
    incompressibleTwoPhaseMixturePCM.C

\*---------------------------------------------------------------------------*/

#ifndef incompressibleTwoPhaseMixturePCM_H
#define incompressibleTwoPhaseMixturePCM_H

#include "incompressible/transportModelPCM/transportModelPCM.H"
#include "incompressible/viscosityModels/viscosityModel/viscosityModel.H"
#include "twoPhaseMixture.H"
#include "IOdictionary.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class incompressibleTwoPhaseMixturePCM Declaration
\*---------------------------------------------------------------------------*/

class incompressibleTwoPhaseMixturePCM
:
    public IOdictionary,
    public transportModelPCM,
    public twoPhaseMixture
{
protected:

    // Protected data

        autoPtr<viscosityModel> nuModel1_;
        autoPtr<viscosityModel> nuModel2_;

        dimensionedScalar rho1_;
        dimensionedScalar rho2_;
        dimensionedScalar cp1_;
        dimensionedScalar cp2_;
        dimensionedScalar k1_;
        dimensionedScalar k2_;
        dimensionedScalar L1_;
        dimensionedScalar L2_;
        dimensionedScalar T1_;
        dimensionedScalar T2_;

        const volVectorField& U_;
        const surfaceScalarField& phi_;

        volScalarField nu_;
        volScalarField Cp_;
        volScalarField k_;
        volScalarField L_;


    // Protected Member Functions

        //- Calculate and return the laminar viscosity
        void calcNu();


public:

    TypeName("incompressibleTwoPhaseMixturePCM");


    // Constructors

        //- Construct from components
        incompressibleTwoPhaseMixturePCM
        (
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    virtual ~incompressibleTwoPhaseMixturePCM()
    {}


    // Member Functions

        //- Return const-access to phase1 viscosityModel
        const viscosityModel& nuModel1() const
        {
            return nuModel1_();
        }

        //- Return const-access to phase2 viscosityModel
        const viscosityModel& nuModel2() const
        {
            return nuModel2_();
        }

        //- Return const-access to phase1 density
        const dimensionedScalar& rho1() const
        {
            return rho1_;
        }

        //- Return const-access to phase2 density
        const dimensionedScalar& rho2() const
        {
            return rho2_;
        };
        //- Return const-access to phase1 density
        const dimensionedScalar& cp1() const
        {
            return cp1_;
        }

        //- Return const-access to phase2 density
        const dimensionedScalar& cp2() const
        {
            return cp2_;
        };

        //- Return const-access to phase1 density
        const dimensionedScalar& k1() const
        {
            return k1_;
        }

        //- Return const-access to phase2 density
        const dimensionedScalar& k2() const
        {
            return k2_;
        };

        //- Return const-access to phase1 density
        const dimensionedScalar& L1() const
        {
            return L1_;
        }

        //- Return const-access to phase2 density
        const dimensionedScalar& L2() const
        {
            return L2_;
        };

        //- Return const-access to phase1 density
        const dimensionedScalar& T1() const
        {
            return T1_;
        }

        //- Return const-access to phase2 density
        const dimensionedScalar& T2() const
        {
            return T2_;
        };

        //- Return const-access to the mixture velocity
        const volVectorField& U() const
        {
            return U_;
        }

        //- Return the dynamic laminar viscosity
        tmp<volScalarField> mu() const;

        //- Return the face-interpolated dynamic laminar viscosity
        tmp<surfaceScalarField> muf() const;

        //- Return the kinematic laminar viscosity
        virtual tmp<volScalarField> nu() const
        {
            return nu_;
        }
        //- Return the kinematic laminar viscosity
        virtual tmp<volScalarField> Cp() const
        {
            return Cp_;
        }
        //- Return the kinematic laminar viscosity
        virtual tmp<volScalarField> k() const
        {
            return k_;
        }

        //- Return the kinematic laminar viscosity
        virtual tmp<volScalarField> L() const
        {
            return L_;
        }

        //- Return the laminar viscosity for patch
        virtual tmp<scalarField> nu(const label patchi) const
        {
            return nu_.boundaryField()[patchi];
        }
        //- Return the laminar viscosity for patch
        virtual tmp<scalarField> Cp(const label patchi) const
        {
            return Cp_.boundaryField()[patchi];
        }
        //- Return the laminar viscosity for patch
        virtual tmp<scalarField> k(const label patchi) const
        {
            return k_.boundaryField()[patchi];
        }

        //- Return the face-interpolated kinematic laminar viscosity
        tmp<surfaceScalarField> nuf() const;

        //- Correct the laminar viscosity
        virtual void correct()
        {
            calcNu();
        }

        //- Read base transportProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //