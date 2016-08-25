This is the Impala Release Process section of the Engineering Release Process document.
This version includes sections on preparing Impala for -rc releases, e.g., rocana 1.5.0-rc1, etc.
Release Impala releases for every Rocana rc release is deprecated,
but it's useful to keep this information in case we need to revive this practice.

Also note that current process may have changed a bit, too,
for example, the Jenkins jobs have probably continued to evolve,
so this document may need to be updated to reflect that.


```
Impala Release Process

Conventions
${IMPALA_BASE}
The base impala version, e.g., 
2.2.0
${IMPALA_VERSION}
The impala version we’re getting ready to release, e.g., 
2.2.0-hdp2.3.0-rocana1.5.0 ,
2.2.0-hdp2.3.0-rocana1.5.0-rc1 , etc.

Build Environments
CentOS 6: impala-dev-el6.int.rocana.com
CentOS 7: impala-dev-el7.int.rocana.com

impala-dev-el6.int.rocana.com: All users share one git repo at /root/Impala, and you’ll probably want to edit /root/.gitconfig and also forward your ssh keys to the root user so it’s easy to work with github, e.g.,
when you originally ssh to the machine do this: ssh -A impala-dev-el6.int.rocana.com
when switching to the root user use: sudo -E su

impala-dev-el7.int.rocana.com: Each user gets their own git repo (git clone https://github.com/scalingdata/Impala) and sets up their own ~/.gitconfig and ssh keys.

Section 0: Obtain a valid Jira ticket number against project “ROCANA”. This ticket number will be of the form “ROCANA-nnnn”, where “nnnn” is some number. This ticket number will be used later in this process for committing the Version update.

Section 1: Create the branch, set the version number and tag the branch

Check out the appropriate release branch locally, creating it if it doesn’t already exist:
For RCs built off of rocana-master, i.e., before code freeze date, create a release branch off of rocana-master for the release named release/${IMPALA_VERSION}-rc${RC_VERSION}:
git clone git@github.com:scalingdata/Impala
cd Impala
git checkout rocana-master
git pull
git checkout -b release/${IMPALA_VERSION}-rc${RC_VERSION}
For RCs built off of a release branch, i.e., after code freeze date, create a release branch off of rocana-master for the release named release/${IMPALA_VERSION}:
git clone git@github.com:scalingdata/Impala
cd Impala
git checkout rocana-master
git pull
git checkout -b release/${IMPALA_VERSION}
Create a development branch for updating the version number. Note that “ROCANA-nnnn” is the Jira number obtained earlier.
git checkout -b ROCANA-nnnn
source bin/impala-config.sh


Follow these steps (section: “Update the Version Number for a New Release”)
For Release Candidates the new version number should be ${IMPALA_VERSION}-rc${RC_VERSION}:
sed -i~ '/^VERSION=/s/=.*/=${IMPALA_VERSION}-rc${RC_VERSION}/' \
bin/save-version.sh &&
git add bin/save-version.sh &&
git commit -m “ROCANA-nnnn: Update version to ${IMPALA_VERSION}-rc${RC_VERSION}” &&
git push origin ROCANA-nnnn
For Releases the new version number should be
${IMPALA_VERSION}:
sed -i~ '/^VERSION=/s/=.*/=${IMPALA_VERSION}/' \
bin/save-version.sh &&
git add bin/save-version.sh &&
git commit -m “ROCANA-nnnn: Update version to ${IMPALA_VERSION}-rc${RC_VERSION}” &&
git push origin ROCANA-nnnn
On Github, create a PR for the version number change.
NB: The PR should seek to merge from ROCANA-nnnn to the release branch, e.g., release/${IMPALA_VERSION}-rc${RC_VERSION} or release/${IMPALA_VERSION}
After review, merge PR into the release branch (NOT to rocana-master).
After merging the PR on Github, check out the release branch and pull in the changes:
For Release Candidates:
git checkout release/${IMPALA_VERSION}-rc${RC_VERSION}
git pull origin release/${IMPALA_VERSION}-rc${RC_VERSION}
For Releases:
git checkout release/${IMPALA_VERSION}
git pull origin release/${IMPALA_VERSION}
Create a tag and push it Github:
For Release Candidates:
git tag -a impala-${IMPALA_VERSION}-rc${RC_VERSION} \
-m impala-${IMPALA_VERSION}-rc${RC_VERSION} ;
git push origin impala-${IMPALA_VERSION}-rc${RC_VERSION}
For Releases:
git tag -a impala-${IMPALA_VERSION} \
-m impala-${IMPALA_VERSION} ;
git push origin impala-${IMPALA_VERSION}


Section 2: Create the source tarball and upload to S3
(NOTE: Most of this section (steps 8 - 12 (upload to AWS & delete tar.gz from build server) is done by Jenkins job “impala-release-tar” - Step 13 (If this is final release...bump rocana version to next SNAPSHOT) still needs to be done by hand)
Create a tarball with the contents of the tag:
For Release Candidates:
git archive --format=tar --prefix=impala-${IMPALA_BASE}/ \ impala-${IMPALA_VERSION}-rc${RC_VERSION} ;
gzip impala-${IMPALA_VERSION}-rc${RC_VERSION}.tar
For Releases:
git archive --format=tar --prefix=impala-${IMPALA_BASE}/ \ impala-${IMPALA_VERSION} ;
gzip impala-${IMPALA_VERSION}.tar
Install and configure aws cli.
Run aws configure
Enter your AWS Access Key Id and Secret Access Key
Default region name: us-west-2
Leave ‘Default output format’ blank.
Upload impala-${IMPALA_VERSION}.tar.gz to S3 in the scalingdata-infrastructure bucket under packages/src.
NB: Make sure the file is downloadable by Everyone.
Example command:
aws s3 cp impala-${IMPALA_VERSION}.tar.gz "s3://scalingdata-infrastructure/packages/src/" --acl public-read --include "*"
NB: Wait for this to finish before building Impala in the next section.
Login to the AWS Console (web UI) and verify the file exists in S3
Now that it’s in S3 we can delete the tar.gz from the build server
If this is a final release (not a release candidate) checkout rocana-master and bump the rocana version to the next SNAPSHOT , e.g.,
2.2.0-hdp2.3.0-rocana1.3.0 
becomes 
2.2.0-hdp2.3.0-rocana1.4.0-SNAPSHOT
Create a PR and merge this change to rocana-master.

Section 3: Set the version in the cdh-package project
Run these steps on the same build server as the previous section.
Clone scalingdata/cdh-package and check out base branch:
git clone git@github.com:scalingdata/cdh-package
cd cdh-package
git checkout impala2.2.0-hdp2.3.0-rocana
Create the appropriate release branch or create it if it doesn’t already exist:
git checkout -b release/${IMPALA_VERSION}
Or for Release Candidates:
git checkout -b release/${IMPALA_VERSION}-rc${RC_VERSION}
Check out the appropriate release branch:
git checkout release/${IMPALA_VERSION}
Or for Release Candidates:
git checkout release/${IMPALA_VERSION}-rc${RC_VERSION}
Push the new branch back to Github:
git push origin release/${IMPALA_VERSION}
Or for Release Candidates:
git push origin release/${IMPALA_VERSION}-rc${RC_VERSION}
Create a branch on which to update the version information:
git checkout -b ROCANA-nnnn
Edit bigtop.mk and fix version numbers as necessary for the below properties:
IMPALA_BASE_VERSION
IMPALA_PKG_VERSION
Note that for Release Candidates, this does not include the rc number, e.g.,
it does not end in something like -rc1
For Release Candidates:
IMPALA_RELEASE_VERSION=0.rc${RC_VERSION}
For Releases:
IMPALA_RELEASE_VERSION=1
IMPALA_TARBALL_SRC
Note that for Release Candidates, this does include the rc number, e.g.,
Impala-2.2.0-hdp2.3.0-rocana1.5.0-rc1.tar.gz
Use Git to commit the changes, and push the branch to Github:
git add bigtop.mk
git commit -m “ROCANA-nnnn: Update to version ${IMPALA_VERSION}”
git push origin ROCANA-nnnn
Create a Pull Request, and get it approved. Once approved, merge the fix back to the release branch and push the change to github. Note that we need to be careful about the proper origin, what branch we are merging to, etc.
After merging the Pull Request, pull the changes down to the release branch:
git checkout release/${IMPALA_VERSION}
git pull
Or for Release Candidates:
git checkout release/${IMPALA_VERSION}-rc${RC_VERSION}
git pull


On the release branch create a tag:
git tag -a impala-${IMPALA_VERSION} \
        -m impala-${IMPALA_VERSION}
Or for Release Candidates:
git tag -a impala-${IMPALA_VERSION}-rc${RC_VERSION} \
        -m impala-${IMPALA_VERSION}-rc${RC_VERSION}
Push the tag to github:
git push origin impala-${IMPALA_VERSION}
Or for Release Candidates:
git push origin impala-${IMPALA_VERSION}-rc${RC_VERSION}

Section 4: Then run these steps for each of the build servers:
(NOTE: These steps are being replaced by Jenkins jobs impala-build-centos6 & impala-build-centos7 - this section will be streamlined accordingly)
If you haven’t already: checkout the appropriate release branch from https://github.com/scalingdata/cdh-package:
release/${IMPALA_VERSION}
Make sure you’ve already followed these prerequisites.
If the machine has built Impala RPMs before then you know this has already been done.
Build:
make realclean
make bigtop-utils-rpm
make impala-rpm
This will take around an hour on CentOS 6 and 1 hour 15 min on CentOS 7.
Configure AWS cli (see instructions several steps above).
The artifacts are built in the directories output/bigtop-utils/ and output/impala/. Upload all of the artifacts named *.rpm to the releases.rocana.com S3 bucket under the impala/${IMPALA_VERSION} directory (impala/${IMPALA_VERSION}-rc${RC_VERSION} for release candidates).
Example commands:
cd cdh-package/output/bigtop-utils ;
aws s3 cp . "s3://releases.rocana.com/impala/${IMPALA_VERSION}/" \
--acl public-read --recursive --include "*" ;
cd ../impala ;
aws s3 cp . "s3://releases.rocana.com/impala/${IMPALA_VERSION}/" \
--acl public-read --recursive --include "*"



```
