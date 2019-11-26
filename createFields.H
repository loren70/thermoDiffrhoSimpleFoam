Info<< "Reading thermophysical properties\n" << endl;

autoPtr<thermoDiffrhoThermo> pThermo
(
    thermoDiffrhoThermo::New(mesh)
);
thermoDiffrhoThermo& thermo = pThermo();
thermo.validate(args.executable(), "h", "e");

volScalarField& p = thermo.p();

volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    thermo.rho()
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

//Reference component mass fraction 
volScalarField Ca
(
    IOobject
    (
        "Ca",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh
);
//Second component mass fraction
volScalarField Cb
(
    IOobject
    (
        "Cb",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    1-Ca
);

#include "compressibleCreatePhi.H"

pressureControl pressureControl(p, rho, simple.dict());

mesh.setFluxRequired(p.name());

Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New
    (
        rho,
        U,
        phi,
        thermo
    )
);

dimensionedScalar initialMass = fvc::domainIntegrate(rho);

Info<< "Reading initialMassFraction of Ca\n" << endl;
 
IOdictionary initialMassFraction
(
    IOobject
    (
        "initialMassFraction",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

Info<< "Reading initial mass fraction of component 'a'\n" << endl;
 
dimensionedScalar Ca0
(
   "Ca0",  dimensionSet(0, 0, 0, 0, 0, 0 ,0), initialMassFraction
);
    
/*    
Info<< "Reading mass diffusivity Dab\n" << endl;
 
dimensionedScalar Dab
(
   "Dab",  dimViscosity, transportProperties
);
    
     
Info<< "Reading thermodiffusion coefficient DT\n" << endl;
 
dimensionedScalar DT
(
    "DT", dimViscosity/dimTemperature, transportProperties
);
*/

#include "createMRF.H"
#include "createFvOptions.H"
