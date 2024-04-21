project('app', 'c', subproject_dir: 'lib', default_options: [
	'buildtype=debug', 
	'c_std=gnu11', 
	'b_lto=false',
	'default_library=static',
	'csp:packet_padding_bytes=42', 
	'csp:buffer_count=1000',
	'csp:buffer_size=2048',
	'csp:conn_max=20',
	'csp:conn_rxqueue_len=1000',
	'csp:qfifo_len=1000',
	'csp:rdp_max_window=1000',
	'csp:port_max_bind=16',
	'csp:use_rtable=true',
	'param:have_fopen=true', 
	'param:collector=false', 
	'param:list_dynamic=true',
	'csp_proc:slash=false',
	'csp_proc:freertos=false',
	'csp_proc:posix=true',
	'csp_proc:proc_runtime=true',
	'csp_proc:RESERVED_PROC_SLOTS=10',
])

add_global_arguments('-I../include', language: 'c')

sources = files([
	'src/main.c',
	'src/param_config.c',
	'src/hooks.c',
    'src/serial.c',
	'src/gpio.c',
])

fs = import('fs') 

# Add RPI3 / RPI4 flag if we are on a Raspberry Pi
if fs.exists('/sys/firmware/devicetree/base/model')
	model = fs.read('/sys/firmware/devicetree/base/model')
	if model.contains('Raspberry Pi 3')
		add_global_arguments('-DRPI3', language: 'c')
	endif
	if model.contains('Raspberry Pi 4')
		add_global_arguments('-DRPI4', language: 'c')
	endif
endif


deps = []
deps += dependency('csp',      fallback: ['csp', 'csp_dep'], required: true).as_link_whole()
deps += dependency('param',    fallback: ['param', 'param_dep'], required: true).as_link_whole()
deps += dependency('csp_proc', fallback: ['csp_proc', 'csp_proc_dep'], required: true).as_link_whole()

csp_app = executable('app', sources,
	dependencies : deps,
	install : true,
	link_args : ['-Wl,--export-dynamic', '-ldl'],
)

