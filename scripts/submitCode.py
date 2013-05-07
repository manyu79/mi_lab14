#
#   SubmitCode.py
#   Author: Erin Fischell
#   Date: February 27, 2013
#   Laboratory for Autonomous Marine Sensing Systems, MIT
#
#   Student submission code for 2.680                                     
#   This script should be located along with a submitCode.sh script
#   in a moos-ivp-extend directory
#   run submitCode.py with the command:
#   python submitCode.py
#
#   Output: a tar'ed file ../<kerbosid>_lab<labnumber>.tar
#
import sys
import os
class SubmitInfo:
        
    def __init__(self,in_data):
        #initialize object
        
        self.data={}
        for row in in_data:
            attr=str(row[0])
            val=row[1]
            self.data[attr]=val
        
        self.data['moosdir']= os.getcwd().split('/')[-1]
        self.data['moospath']='../'+os.getcwd().split('/')[-1]
        
    def __repr__(self):
        
        return repr(self.data)
    def __getitem__(self, item):
        return self.data[item]
    def __setitem__(self, key,item):
        self.data[key]=item
    def keys(self):
        return self.data.keys()
    
def getVarsFromFile(filename):
    
    f = open(filename,'r')
    attrList=[]
    for line in f:
        
        if line[0]!='#':
            items=line.split('=')
            key = items[0]
            val=items[1].split('\n')[0]
            attrList.append([key,val])
        
    existFields=SubmitInfo(attrList)
    return existFields

def checkOldSubmit():
    #check to see if there is an existing temporary file
    #it will put it in data/
    os.system('mkdir -p data')
    filename='data/submitData.txt'
    try:
        open(filename,'r')
        out=getVarsFromFile(filename)
    except IOError as e:
        out = None
        pass

    return out

def getInputs(useOld=True):

    oldinput=checkOldSubmit()
    newInput=True
    if oldinput != None:
        if 'email' in oldinput.keys() and 'lname' in oldinput.keys() and 'fname' in oldinput.keys():
            email=oldinput['email']
            
            lname=oldinput['lname']
            fname=oldinput['fname']
            newInput=False
    if newInput or not useOld:   
        email=raw_input('Please input your E-mail address: ')
        fname=raw_input('Please input your First Name: ')
        lname=raw_input('Please input your Last Name: ')
    username=email.split('@')[0]
    print '\nChoices for mission directories are: '
    mylist=os.listdir('missions/')
    for el in mylist:
        if el[0]!='.':
            print el
    missiondir=raw_input('Select which one you want: ')
    #next, confirm if the one they wanted exists:
    missexists=os.path.isdir('missions/'+missiondir)
    while not missexists:
        print '\nYou must specify an existing missiondir.  Below are your choices.  Please try again:'
        print mylist
        missiondir=raw_input('Select one of the above:')
        missexists=os.path.isdir('missions/'+missiondir)
    storefile=open('data/submitData.txt','w')
    print >> storefile,'username=' + str(username)
    print >> storefile,'email=' + str(email)
    print >> storefile,'fname='+str(fname)
    print >> storefile,'lname='+str(lname)
    storefile.close()
    #copy the missiondir specified to missions/current
    os.system('mkdir missions/current; cp -r missions/'+missiondir +'/*.bhv missions/' + missiondir +'/*.moos missions/'+missiondir + '/*.sh missions/current/')
    #moosdir=raw_input('Please input the path to your MOOS directory you want zipped up and sent (e.g. /home/efischell/moos-ivp-extend/):')
    existFields=SubmitInfo([['username',username],['passcode',''],['email',email],['missiondir',missiondir],['lname',lname],['fname',fname]])
    return existFields

def confirmMOOSStructure(moosdir):
    #confirm that things are in the correct places:
    #todo: add other path existence checks here.
    good=False
    srcexists=os.path.isdir(moosdir +'/src')
    cmakesrcexists=os.path.isfile(moosdir +'/src/CMakeLists.txt')
    missexists=os.path.isdir(moosdir+'/missions/current')
    buildexists = os.path.isfile(moosdir + '/build.sh')
    nummoos=0
    moosfile=''
    moosfilegood=False
    for myfile in os.listdir(str(moosdir+'/missions/current/')):
        if 'moos' in myfile:
            nummoos=nummoos+1
            moosfile= myfile
    if nummoos>=1:
        moosfilegood=True
    
    if srcexists and missexists and cmakesrcexists and moosfilegood:
        print '\nStructure Appears to be Correct...'
        good=True
    if not srcexists:
        raise Exception('Error in the structure of your moos-ivp-extend tree.  You must have a src/ directory')
    if not missexists:
        raise Exception('Error in the structure of your moos-ivp-extend tree.  You must have a missions/ directory')
    if not cmakesrcexists:
        raise Exception('Error in the structure of your moos-ivp-extend tree.  You must have a file src/CMakeLists.txt')
    if not buildexists:
        raise Exception('Error in the structure of your moos-ivp-extend tree.  You must have a build.sh file in your moos-ivp-extend directory.')
    if not moosfilegood:
        raise Exception('Error in the structure of your moos-ivp-extend tree.  You must have at least one .moos file in the missions directory you specify.')
    
    return good

def submitCode(existFields,lab_number,run_scp):
    #first, tar up the file: (note that in the actual version directory will be replaced w/ moos-ivp-external or whatever
    username=existFields['username']
    moosdir = existFields['moosdir']
    moospath=existFields['moospath']
    tarfile='../'+username+'_lab' + str(lab_number)+'.tar.gz'
    #confirm that the required structure is there (there must be a src directory and a missions directory)
    good=confirmMOOSStructure(moospath)
    print '\nTarring your directory now...'
    
    #todo: construct the excludes from a list of directories/files to exclude
    os.system('tar --directory=../ --exclude ".svn" --exclude "'+moosdir+'/bin/*" ' +'--exclude "'+moosdir+ '/build/*" '+'--exclude "'+moosdir+ '/lib/*" '+'--exclude "'+moosdir+ '/missions/*/LOG*" -czf ' + tarfile + ' ' + moosdir)
    #now that directory is zipped up, you need to submit it:
    if run_scp:
        print '''Running scp on your tar'ed file...'''
        runSCP(tarfile,existFields)
    
def runSCP(tarFileName,existFields):
    #run scp, using the username and a static password already entered.  This will send the zipped folder.
    #todo: implement the scp function in this@128.30.31.10:

    success=os.system('scp '+ tarFileName+' student@128.30.31.48: <<student')
    if success==0:
        return True
    else:
        print '''Error automatically scp'ing your file, make sure you are connected to the internet and try again.'''
        return False
    pass

def main(run_scp):
    os.chdir('../')
    existFields=getInputs()
    #ask if the resulting values are correct:
    
    good=False
    while not good:
        print '\nUsing the following fields: email = '+str(existFields['email']) + ', username = ' + str(existFields['username']) + ', missions_dir=' + str(existFields['missiondir']) + ', moosdir='+str(existFields['moosdir'])
        new=raw_input('\nAre these Correct? Y/N ' )
        if new=='N' or new == 'n':
            existFields=getInputs(False)
        else:
            good=True
            
    #request lab number        
    lab_number=raw_input('\nPlease Input the lab number: ')
    #run submit code
    submitCode(existFields,lab_number,run_scp)
    #clean up
    os.system('rm -r missions/current')

    print '\nDone!'
    
#check for flag(s):
run_scp=True
if (len(sys.argv)>1):

    if sys.argv[1]=='-tar_only':
        print 'Tarring file only without scp'
        main(False)
    elif sys.argv[1]=='-h':
        pass
    else:
        print 'unrecognized flag: only allowable are -h (help) or -tar_only (tar only, no scp)'
else:
    main(True)
        

