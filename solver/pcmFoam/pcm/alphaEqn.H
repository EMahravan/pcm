
forAll(alpha1,celli)
{
     if(T[celli]<mixture.T1().value())
         alpha1[celli]=1.;
     else if(T[celli]<=mixture.T2().value())
         alpha1[celli]=(mixture.T2().value()-T[celli])/(mixture.T2().value()-mixture.T1().value());
     else
         alpha1[celli]=0;
}


volScalarField::Boundary& alpha1bf = alpha1.boundaryFieldRef();
volScalarField::Boundary& Tbf = T.boundaryFieldRef();

forAll(alpha1bf, patchi)
{
    fvPatchScalarField& alpha1p = alpha1bf[patchi];
    fvPatchScalarField& Tp = Tbf[patchi];
    
    forAll(alpha1p, facei)
    {
        if(Tp[facei]<mixture.T1().value())
         alpha1p[facei]=1.;
         else if(Tp[facei]<=mixture.T2().value())
             alpha1p[facei]=(mixture.T2().value()-Tp[facei])/(mixture.T2().value()-mixture.T1().value());
         else
             alpha1p[facei]=0;
    }
}
