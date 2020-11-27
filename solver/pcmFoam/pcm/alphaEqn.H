
forAll(alpha1,celli)
{
     if(T[celli]<mixture.T1().value())
         alpha1[celli]=0.;
     else if(T[celli]<=mixture.T2().value())
         alpha1[celli]=(T[celli]-mixture.T1().value())/(mixture.T2().value()-mixture.T1().value());
     else
         alpha1[celli]=1;
}
