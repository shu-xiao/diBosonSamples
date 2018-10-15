# diBosonSamples
This is the script to quickly generate the gridpacks ([twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/QuickGuideMadGraph5aMCatNLO#Create_the_gridpacks_for_each_pr)) for diboson resonance model, q q~ > X > h h

### Prerequisites
* Python 2.X
* [MadGraph5_aMCatNLO](https://launchpad.net/mg5amcnlo)

### Install and Test
here is the way to install and do a simple test
after executing python script, gridpacks will be generated in several minutes

```
cd /path/to/genproductions/bin/MadGraph5_aMCatNLO/
git clone https://github.com/shu-xiao/diBosonSamples.git
cp diBosonSamples/genGridpack_diboson.py .
cp -r diBosonSamples/cards/ . 
rm -rf diBosonSamples/
# test
python genGridpack_diboson.py
```

### Structure
To generate gridpack, you need to prepare some cards for gridpack
[here](cards/) are the template cards for Radion and BulkGraviton
[python script](genGridpack_diboson.py) copies BulkGraviton and Radion template cards and replace the parameters of the cards
after preparing cards, jobs to generate gridpack will be sent to batch system and it will finished in several minutes 

### How do I use it?
* How to generate large number of samples

please change this [line](genGridpack_diboson.py#L12), the gridpack with the mass point in array will be generated

* How to change the width

please change this [line](genGridpack_diboson.py#L19-L20), width can vary with resonance mass or be fixed  
