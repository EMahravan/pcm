/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) YEAR OpenFOAM Foundation
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

#include "mixedFvPatchFieldTemplate.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "unitConversion.H"
//{{{ begin codeInclude

//}}} end codeInclude


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

//{{{ begin localCode

//}}} end localCode


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

extern "C"
{
    // dynamicCode:
    // SHA1 = 60ac32bccca12931f534489ff3426ea8b6153521
    //
    // unique function name that can be checked if the correct library version
    // has been loaded
    void heatFlux_60ac32bccca12931f534489ff3426ea8b6153521(bool load)
    {
        if (load)
        {
            // code that can be explicitly executed after loading
        }
        else
        {
            // code that can be explicitly executed before unloading
        }
    }
}

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

makeRemovablePatchTypeField
(
    fvPatchScalarField,
    heatFluxMixedValueFvPatchScalarField
);


const char* const heatFluxMixedValueFvPatchScalarField::SHA1sum =
    "60ac32bccca12931f534489ff3426ea8b6153521";


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

heatFluxMixedValueFvPatchScalarField::
heatFluxMixedValueFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchField<scalar>(p, iF)
{
    if (false)
    {
        Info<<"construct heatFlux sha1: 60ac32bccca12931f534489ff3426ea8b6153521"
            " from patch/DimensionedField\n";
    }
}


heatFluxMixedValueFvPatchScalarField::
heatFluxMixedValueFvPatchScalarField
(
    const heatFluxMixedValueFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<scalar>(ptf, p, iF, mapper)
{
    if (false)
    {
        Info<<"construct heatFlux sha1: 60ac32bccca12931f534489ff3426ea8b6153521"
            " from patch/DimensionedField/mapper\n";
    }
}


heatFluxMixedValueFvPatchScalarField::
heatFluxMixedValueFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<scalar>(p, iF, dict)
{
    if (false)
    {
        Info<<"construct heatFlux sha1: 60ac32bccca12931f534489ff3426ea8b6153521"
            " from patch/dictionary\n";
    }
}


heatFluxMixedValueFvPatchScalarField::
heatFluxMixedValueFvPatchScalarField
(
    const heatFluxMixedValueFvPatchScalarField& ptf
)
:
    mixedFvPatchField<scalar>(ptf)
{
    if (false)
    {
        Info<<"construct heatFlux sha1: 60ac32bccca12931f534489ff3426ea8b6153521"
            " as copy\n";
    }
}


heatFluxMixedValueFvPatchScalarField::
heatFluxMixedValueFvPatchScalarField
(
    const heatFluxMixedValueFvPatchScalarField& ptf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchField<scalar>(ptf, iF)
{
    if (false)
    {
        Info<<"construct heatFlux sha1: 60ac32bccca12931f534489ff3426ea8b6153521 "
            "as copy/DimensionedField\n";
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

heatFluxMixedValueFvPatchScalarField::
~heatFluxMixedValueFvPatchScalarField()
{
    if (false)
    {
        Info<<"destroy heatFlux sha1: 60ac32bccca12931f534489ff3426ea8b6153521\n";
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void heatFluxMixedValueFvPatchScalarField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    if (false)
    {
        Info<<"updateCoeffs heatFlux sha1: 60ac32bccca12931f534489ff3426ea8b6153521\n";
    }

//{{{ begin code
    #line 38 "/home/ehsan/OpenFOAM/ehsan-7/pcm/validation/0/pcm/T.boundaryField.pcmRight"
const volScalarField& kappa=this->db().lookupObject<volScalarField>("kappa");
            dimensionedScalar q("heatFlux",dimEnergy/dimArea,250.0);
            Info<<"q="<<q<<endl;
            Info<<"kappa="<<kappa.boundaryField()[this->patch().index()]<<endl;
            
            this->refValue() =0;
            this->refGrad() = q.value()/(kappa.boundaryField()[this->patch().index()]);
            this->valueFraction() = 0.0;
//}}} end code

    this->mixedFvPatchField<scalar>::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //

