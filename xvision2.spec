%define name XVision2
%define version 2.2
%define release 1
%define sourcepkg %{name}-%{version}.tar.gz

Summary: Dario's XVision2 image processing library
Name: %{name}
Version: %{version}
Release: %{release}
Source0: %{sourcepkg}
URL: http://download.visual-navigation.com/%{sourcepkg}
License: none
Packager: Darius Burschka <burschka@cs.tum.edu>
Group: Development/Libraries
BuildRoot: /var/tmp/%{name}-buildroot
Prefix: /usr/local
Requires: libdc1394-devel,libraw1394-devel,ffmpeg-devel

%description
Greg Hager's and Darius Burschka's XVision2 library with efficient
implementations of tracking algorithms for efficient processing on
embedded and mobile systems.

%prep
rm -rf ${RPM_BUILD_ROOT}

%setup -q

%build
CFLAGS="$RPM_OPT_FLAGS" make config
make

%install
rm -rf $RPM_BUILD_ROOT
make install prefix=$RPM_BUILD_ROOT/%{prefix}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc 
%{prefix}/*

%changelog

# end of file
