project('box', 'c', default_options: [ 'c_std=c89', 'werror=true' ])

lib = static_library(
	meson.project_name(),
	'src/box.c',
	'src/capture.c',
	include_directories: include_directories('include'),
	c_args: [ '-pedantic-errors' ],
)

dep = declare_dependency(
	link_with: lib,
	include_directories: include_directories('include'),
)

meson.override_dependency('box', dep)
