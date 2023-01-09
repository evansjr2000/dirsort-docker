#define MAXSYSFILES 2000000
#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4
#define TRUE 1
#define FALSE 0
/*3:*/
#line 51 "dirsort.web"

/*4:*/
#line 59 "dirsort.web"

#include        <sys/types.h> 
#include        <sys/stat.h> 
#include        <dirent.h> 
#include        <limits.h> 
#include        "ourhdr.h"


/*:4*//*16:*/
#line 285 "dirsort.web"

#include <pwd.h> 

/*:16*/
#line 52 "dirsort.web"

/*5:*/
#line 69 "dirsort.web"

typedef int(Myfunc)(const char*,const struct stat*,int);
static long filesize[MAXSYSFILES],indices[MAXSYSFILES];
static char*filenames[MAXSYSFILES];

/*:5*/
#line 53 "dirsort.web"

/*6:*/
#line 75 "dirsort.web"

static Myfunc myfunc;
static int myftw(char*,Myfunc*);
static int dopath(Myfunc*);
static long nreg,ndir,nblk,nchr,nfifo,nslink,nsock,ntot;


/*:6*//*13:*/
#line 242 "dirsort.web"

long ureg;
int debug= 0;

/*:13*//*15:*/
#line 277 "dirsort.web"

extern char*optarg;
int errflg,yrcnt,c;
int do_uid= FALSE;
uid_t uid;
struct passwd*pwd;

/*:15*//*18:*/
#line 302 "dirsort.web"

long j;

/*:18*//*20:*/
#line 335 "dirsort.web"

long tsize;
double fsize;

/*:20*/
#line 54 "dirsort.web"

/*9:*/
#line 122 "dirsort.web"

static char*fullpath;
static int
myftw(char*pathname,Myfunc*func)
{
fullpath= path_alloc(NULL);

strcpy(fullpath,pathname);
return(dopath(func));
}

/*:9*//*10:*/
#line 138 "dirsort.web"

static int
dopath(Myfunc*func)
{
struct stat statbuf;
struct dirent*dirp;
DIR*dp;
int ret;
char*ptr;
if(lstat(fullpath,&statbuf)<0)
return(func(fullpath,&statbuf,FTW_NS));

if(S_ISDIR(statbuf.st_mode)==0){
if(debug){
printf("debug>fullpath = %s.\n",fullpath);
}
return(func(fullpath,&statbuf,FTW_F));
}




if(debug){
printf("debug>fullpath = %s.\n",fullpath);
}
if((ret= func(fullpath,&statbuf,FTW_D))!=0)
return(ret);
ptr= fullpath+strlen(fullpath);
*ptr++= '/';
*ptr= 0;
if((dp= opendir(fullpath))==NULL)
return(func(fullpath,&statbuf,FTW_DNR));

while((dirp= readdir(dp))!=NULL){
if(strcmp(dirp->d_name,".")==0||
strcmp(dirp->d_name,"..")==0)
continue;
strcpy(ptr,dirp->d_name);
if((ret= dopath(func))!=0)
break;
}
ptr[-1]= 0;
if(closedir(dp)<0)
err_ret("can't close directory %s",fullpath);
return(ret);
}

/*:10*//*11:*/
#line 186 "dirsort.web"

static int
myfunc(const char*pathname,const struct stat*statptr,int type)
{
switch(type){
case FTW_F:
switch(statptr->st_mode&S_IFMT){
case S_IFREG:nreg++;
/*12:*/
#line 227 "dirsort.web"

if((uid==statptr->st_uid)||(!do_uid)){
ureg++;
filenames[ureg]= malloc(strlen(pathname)+1);
strcpy(filenames[ureg],pathname);
filesize[ureg]= statptr->st_size;
if(debug){
printf("Debug> Process regular file.\n");
printf("Debug> Count is %ld.\n",ureg);
printf("Debug> Filename is %s.\n",filenames[ureg]);
printf("Debug> Filesize is %ld.\n",filesize[ureg]);
}
}

/*:12*/
#line 194 "dirsort.web"

break;
case S_IFBLK:nblk++;break;
case S_IFCHR:nchr++;break;
case S_IFIFO:nfifo++;break;
case S_IFLNK:nslink++;break;
case S_IFSOCK:nsock++;break;
case S_IFDIR:
err_dump("for S_IFDIR for %s",pathname);

}
break;

case FTW_D:
ndir++;
break;

case FTW_DNR:
err_ret("can't read directory %s",pathname);
break;

case FTW_NS:
err_ret("stat error for %s",pathname);
break;

default:
err_dump("unknown type %d for pathname %s",type,pathname);
}

return(0);
}

/*:11*//*21:*/
#line 340 "dirsort.web"

void indexx(n,arrin,indx)
long n,indx[];
long arrin[];
{
long l,j,ir,indxt,i;
long q;

for(j= 1;j<=n;j++)indx[j]= j;
l= (n>>1)+1;
ir= n;
for(;;){
if(l> 1)
q= arrin[(indxt= indx[--l])];
else{
q= arrin[(indxt= indx[ir])];
indx[ir]= indx[1];
if(--ir==1){
indx[1]= indxt;
return;
}
}
i= l;
j= l<<1;
while(j<=ir){
if(j<ir&&arrin[indx[j]]<arrin[indx[j+1]])j++;
if(q<arrin[indx[j]]){
indx[i]= indx[j];
j+= (i= j);
}
else j= ir+1;
}
indx[i]= indxt;
}
}

/*:21*/
#line 55 "dirsort.web"

/*7:*/
#line 84 "dirsort.web"


int
main(int argc,char*argv[])
{
int ret;
extern int optind;
/*14:*/
#line 247 "dirsort.web"

yrcnt= 50;
while((c= getopt(argc,argv,"n:u:"))!=EOF){
switch(c){
case'n':
yrcnt= atoi(optarg);
break;
case'u':
pwd= getpwnam(optarg);
if(pwd==NULL){
fprintf(stderr,"Error: username %s not found.\n",optarg);
exit(2);
}
else{
uid= pwd->pw_uid;
do_uid= TRUE;
}
break;
case'?':
errflg++;
}}
if(optind>=argc)
errflg++;
if(errflg){
err_quit("usage: dirsort [-n <#>] [-u <username>] path1...");
}

/*:14*/
#line 91 "dirsort.web"

for(;optind<argc;optind++)
ret= myftw(argv[optind],myfunc);
/*8:*/
#line 102 "dirsort.web"

if((ntot= nreg+ndir+nblk+nchr+nfifo+nslink+nsock)==0)
ntot= 1;
printf("regular files  = %7ld, %5.2f %%\n",nreg,nreg*100.0/ntot);
printf("directories    = %7ld, %5.2f %%\n",ndir,ndir*100.0/ntot);
printf("block special  = %7ld, %5.2f %%\n",nblk,nblk*100.0/ntot);
printf("char special   = %7ld, %5.2f %%\n",nchr,nchr*100.0/ntot);
printf("FIFOs          = %7ld, %5.2f %%\n",nfifo,nfifo*100.0/ntot);
printf("symbolic links = %7ld, %5.2f %%\n",nslink,nslink*100.0/ntot);
printf("sockets        = %7ld, %5.2f %%\n",nsock,nsock*100.0/ntot);
if(do_uid)
printf("\n%s files = %7ld, %5.2f %%\n",pwd->pw_name,ureg,ureg*100.0/ntot);

/*:8*/
#line 94 "dirsort.web"

/*17:*/
#line 289 "dirsort.web"

#if defined(DEBUG)
printf("Sorting file sizes.\n");
#endif
#line 293 "dirsort.web"
 if(ureg> 1)
indexx(ureg,filesize,indices);
#if defined(DEBUG)
for(j= 1;j<=ureg;j++){
printf("%s, indices[%ld] = %ld\n",filenames[j],j,indices[j]);
}
#endif
#line 300 "dirsort.web"

/*:17*/
#line 95 "dirsort.web"

/*19:*/
#line 306 "dirsort.web"

#if defined(DEBUG)
printf("Outputting data.\n");
#endif
#line 310 "dirsort.web"
 if(yrcnt> ureg)yrcnt= ureg;
for(j= ureg;j> ureg-yrcnt;j--){
tsize= filesize[indices[j]];
if(tsize> 1000000000){
fsize= tsize/1000000000;
fprintf(stdout,"%3ld. (%4g GB) \t %s \n",(ureg-j+1),
fsize,filenames[indices[j]]);
}
else if(tsize> 1000000){
fsize= tsize/1000000;
fprintf(stdout,"%3ld. (%4g MB) \t %s \n",(ureg-j+1),
fsize,filenames[indices[j]]);
}
else if(tsize> 1000){
fsize= tsize/1000;
fprintf(stdout,"%3ld. (%4g KB) \t %s \n",(ureg-j+1),
fsize,filenames[indices[j]]);
}
else{
fprintf(stdout,"%3ld. (%4ld bytes) \t %s \n",(ureg-j+1),
filesize[indices[j]],filenames[indices[j]]);
}
}

/*:19*/
#line 96 "dirsort.web"

exit(ret);
}


/*:7*/
#line 56 "dirsort.web"


/*:3*/
