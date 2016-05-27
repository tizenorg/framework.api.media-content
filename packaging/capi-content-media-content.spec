Name:       capi-content-media-content
Summary:    A Media content library in SLP C API
Version: 0.2.156
Release:    0
Group:      System/Libraries
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(libmedia-service)
BuildRequires:  pkgconfig(media-thumbnail)
BuildRequires:  pkgconfig(libmedia-utils)
BuildRequires:  pkgconfig(vconf)


Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description


%package devel
Summary:  A Media content library in SLP C API (Development)
Group:    Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel

%prep
%setup -q


%build
export CFLAGS+=" -Wextra -Wno-array-bounds"
export CFLAGS+=" -Wno-ignored-qualifiers -Wno-unused-parameter -Wshadow"
export CFLAGS+=" -Wwrite-strings -Wswitch-default"
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`

%if 0%{?sec_build_binary_debug_enable}
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

#License
mkdir -p %{buildroot}/%{_datadir}/license
cp -rf %{_builddir}/%{name}-%{version}/LICENSE.APLv2.0 %{buildroot}/%{_datadir}/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest capi-content-media-content.manifest
%{_libdir}/libcapi-content-media-content.so.*
#License
%{_datadir}/license/%{name}

%files devel
%{_includedir}/media-content/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-content-media-content.so
