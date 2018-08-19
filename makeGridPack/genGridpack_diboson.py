import os
import shutil

exe = 0
sub = 1
def mkNdir(dirName):
    if not os.path.isdir(dirName): os.mkdir(dirName)


def main():
    widthRatio = 0.1
    #mXList = [600,800,1000,1200,1400,1700,2000,2500] 
    mXList = [3000] 
    for mX in mXList:
        # spin 2, BulkGraviton
        dirName = 'BulkGraviton_hh_M'+str(mX)
        mkNdir('cards/'+dirName)
        print('create '+dirName)
        shutil.copyfile('cards/BulkGraviton_hh_template/tem_run_card.dat','cards/'+dirName+'/'+dirName+'_run_card.dat')
        shutil.copyfile('cards/BulkGraviton_hh_template/tem_extramodels.dat','cards/'+dirName+'/'+dirName+'_extramodels.dat')

        f_proc0 = open('cards/BulkGraviton_hh_template/tem_proc_card.dat','r')
        f_proc1 = open('cards/'+dirName+'/'+dirName+'_proc_card.dat','w')
        for line in f_proc0:
            f_proc1.write(line.replace('BulkGraviton_hh_M5000_test',dirName))
        f_proc0.close()
        f_proc1.close()

        f_cust0 = open('cards/BulkGraviton_hh_template/tem_customizecards.dat','r')
        f_cust1 = open('cards/'+dirName+'/'+dirName+'_customizecards.dat','w')
        for line in f_cust0:
            if line.find('MX') > 0: f_cust1.write(line.replace('MX',str(mX)))
            elif line.find('WX') > 0: f_cust1.write(line.replace('WX',str(mX*widthRatio)))
            else: f_cust1.write(line)
        f_cust0.close()
        f_cust1.close()
        command = './gridpack_generation.sh ' + dirName + ' cards/' + dirName
        subcom = './submit_gridpack_generation.sh 12000 12000 1nw ' + dirName + ' cards/' + dirName + ' 2nd'
        print(command)
        if (exe): os.system(command)
        if (sub): os.system(subcom)

        # spin 0, Radion
        dirName = 'Radion_hh_M'+str(mX)
        mkNdir('cards/'+dirName)
        print('create '+dirName)
        shutil.copyfile('cards/Radion_hh_template/tem_run_card.dat','cards/'+dirName+'/'+dirName+'_run_card.dat')
        shutil.copyfile('cards/Radion_hh_template/tem_extramodels.dat','cards/'+dirName+'/'+dirName+'_extramodels.dat')

        f_proc0 = open('cards/Radion_hh_template/tem_proc_card.dat','r')
        f_proc1 = open('cards/'+dirName+'/'+dirName+'_proc_card.dat','w')
        for line in f_proc0:
            f_proc1.write(line.replace('Radion_hh_M5000_test',dirName))
        f_proc0.close()
        f_proc1.close()

        f_cust0 = open('cards/Radion_hh_template/tem_customizecards.dat','r')
        f_cust1 = open('cards/'+dirName+'/'+dirName+'_customizecards.dat','w')
        for line in f_cust0:
            if line.find('MX') > 0: f_cust1.write(line.replace('MX',str(mX)))
            elif line.find('WX') > 0: f_cust1.write(line.replace('WX',str(mX*widthRatio)))
            else: f_cust1.write(line)
        f_cust0.close()
        f_cust1.close()
        command = './gridpack_generation.sh ' + dirName + ' cards/' + dirName
        subcom = './submit_gridpack_generation.sh 12000 12000 1nw ' + dirName + ' cards/' + dirName + ' 2nd'
        print(command)
        if (exe): os.system(command)
        if (sub): os.system(subcom)

if __name__ == "__main__":
    main()
    #os.system('cp *.tarball.tar.xz ~/public/ZpBaryonic_gridpack_new')
