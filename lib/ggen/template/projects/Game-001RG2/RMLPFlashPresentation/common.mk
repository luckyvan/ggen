# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.


ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)


ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif


#===== Settings shared by all components.
include ../../../LibShared/shared_settings.mk

#===== LDFLAGS - add the flags to the linker command line.
LDFLAGS+=-Wl,--warn-shared-textrel,--fatal-warnings  \
	-Wl,--version-script=../../../$(PRODUCT)/StandardExportedSymbols.def

#===== CCFLAGS - add the flags to the C compiler command line. 
CCFLAGS+=-D__REELS_2__ -Werror

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT)/inc  \
	$(PROJECT_ROOT_LibPresentation)/inc  \
	$(PROJECT_ROOT_LibSlotPresentation)/inc  \
	$(PROJECT_ROOT_LibSlotWinAnimation)/interfaces  \
	$(PROJECT_ROOT_LibSys)/Common/inc

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/src
#===== POST_BUILD - extra steps to do after building the image.
define POST_BUILD
-@$(CP_HOST) $(BUILDNAME) $(AVP_DEVELOPMENT_ROOT)/Games/$(PRODUCT)/$(BUILDNAME)
endef
#===== VERSION_TAG_SO - version tag for SONAME. Use it only if you don't like SONAME_VERSION
override VERSION_TAG_SO=

include $(MKFILES_ROOT)/qmacros.mk
#===== BUILDNAME - exact name (before appending suffixes like .a, .so) of build goal.
BUILDNAME=RMLPFlashPresentation.so
#===== EXTRA_OBJS - object files and libraries contributed additionaly on link/archive step
EXTRA_OBJS+=$(PROJECT_ROOT_LibPresentation)/LibPresentation.a  \
	$(PROJECT_ROOT_LibSys)/LibSys.a  \
	$(PROJECT_ROOT_LibShared)/LibShared.a
include $(QNX_INTERNAL)
postbuild:
	$(POST_BUILD)
include $(MKFILES_ROOT)/qtargets.mk


OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
OPTIMIZE_TYPE_g=none
