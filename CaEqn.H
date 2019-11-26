//Species transport equation in terms of the reference component 'Ca' mass fraction.
fvScalarMatrix CaEqn
(
      fvm::ddt(rho, Ca)
    + fvm::div(phi, Ca)
    - fvm::laplacian(thermo.Dab()*rho, Ca)
);

solve(CaEqn == fvc::laplacian(thermo.DT()*Ca0*(1-Ca0)*rho, thermo.T()));
CaEqn.relax();

