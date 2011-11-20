# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# For each target create a dummy rule so the target does not have to exist


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.angryman.Debug:
/Users/ouj/git/angryman/bin/Debug/angryman.app/Contents/MacOS/angryman:
	/bin/rm -f /Users/ouj/git/angryman/bin/Debug/angryman.app/Contents/MacOS/angryman


PostBuild.angryman.Release:
/Users/ouj/git/angryman/bin/Release/angryman.app/Contents/MacOS/angryman:
	/bin/rm -f /Users/ouj/git/angryman/bin/Release/angryman.app/Contents/MacOS/angryman


PostBuild.angryman.MinSizeRel:
/Users/ouj/git/angryman/bin/MinSizeRel/angryman.app/Contents/MacOS/angryman:
	/bin/rm -f /Users/ouj/git/angryman/bin/MinSizeRel/angryman.app/Contents/MacOS/angryman


PostBuild.angryman.RelWithDebInfo:
/Users/ouj/git/angryman/bin/RelWithDebInfo/angryman.app/Contents/MacOS/angryman:
	/bin/rm -f /Users/ouj/git/angryman/bin/RelWithDebInfo/angryman.app/Contents/MacOS/angryman


