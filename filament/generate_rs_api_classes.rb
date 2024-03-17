require 'set'

class InnerNameSet
	def initialize
		@types_to_prefix = Set[ "TextureDetectCallback", "TextureDetectRoughnessCallback" ]
	end

	def add(name)
		@types_to_prefix.add name
	end

	def prefix_type(type)
		prefixed = if @types_to_prefix.include? type
			"RenderingServer::#{type}"
		else
			type
		end

		prefixed = prefixed.gsub(/<(.*?)>/) { "<#{prefix_type($1)}>" }

		prefixed.gsub!(/const\s+(.+?)\s*&/) { "const #{prefix_type($1)} &" }

		prefixed
	end
end

=begin
module PrototypeCommon

	protected

	TYPES_TO_PREFIX = Set[ "TextureLayeredType", "TextureDetectCallback", "TextureDetectRoughnessCallback", "ShaderNativeSourceCode", "SurfaceData", "BlendShapeMode",
	                       "MultimeshTransformFormat", "LightParam", "LightBakeMode", "LightOmniShadowMode", "LightDirectionalShadowMode", "LightDirectionalSkyMode",
	                       "ShadowQuality", "TextureInfo", "SurfaceData", "LightProjectorFilter", "ReflectionProbeUpdateMode", "ReflectionProbeAmbientMode",
	                       "DecalTexture", "DecalFilter", "VoxelGIQuality", "ParticlesMode", "ParticlesTransformAlign", "ParticlesDrawOrder", "ParticlesCollisionType",
	                       "FogVolumeShape", "ViewportScaling3DMode", "ViewportUpdateMode", "ViewportClearMode", "ViewportEnvironmentMode", "CanvasItemTextureFilter",
	                       "CanvasItemTextureRepeat", "ViewportSDFOversize", "ViewportSDFScale", "ViewportMSAA", "ViewportScreenSpaceAA", "ViewportOcclusionCullingBuildQuality",
	                       "ViewportRenderInfoType", "ViewportRenderInfo", "ViewportDebugDraw", "ViewportVRSMode", "SkyMode", "EnvironmentBG", "EnvironmentAmbientSource",
	                       "EnvironmentReflectionSource", "EnvironmentGlowBlendMode", "EnvironmentToneMapper", "EnvironmentSSRRoughnessQuality", "EnvironmentSSAOQuality",
	                       "EnvironmentSSILQuality", "EnvironmentSDFIGYScale", "EnvironmentSDFGIRayCount", "EnvironmentSDFGIFramesToConverge", "EnvironmentSDFGIFramesToUpdateLight",
	                       "SubSurfaceScatteringQuality", "DOFBlurQuality", "]

	def prefix_type(type)
		prefixed = if TYPES_TO_PREFIX.include? type
			"RenderingServer::#{type}"
		else
			type
		end

		prefixed = prefixed.gsub(/<(.*?)>/) { "<#{prefix_type($1)}>" }

		prefixed.gsub!(/const\s+(.+?)\s*&/) { "const #{prefix_type($1)} &" }

		prefixed
	end
end
=end

class PrototypeArgument
	attr_reader :type, :name

	def initialize(arg)
		if !arg.include?(" ")
			@type = arg
			@name = "anonarg"
		else

			unless arg =~ /\A(.+?)\s*([^ &*]*)\Z/
				raise "malformed argument: #{arg.inspect}"
			end

			@type = $1
			@name = $2

			@name.strip!

			if @name.empty?
				@name = "anonarg"
			end
		end
	end
end

class FunctionPrototype
	attr_reader :return_type, :name, :qualifiers, :args

	def initialize(prototype)
		prototype = prototype.dup

		unless prototype =~ /\A(.+?)\s+(.+?)\s*\((.*)\)\s*(const)?\Z/
			raise "malformed prototype: #{prototype.inspect}"
		end

		@return_type = $1
		@name = $2
		@qualifiers = $4

		args = $3

		proto_nesting = 0
		in_default = nil
		ranges_to_delete = []

		args.each_char.with_index do |char, index|
			if char == '('
				proto_nesting += 1
			elsif char == ')'
				proto_nesting -= 1
			end

			if char == '='
				unless in_default.nil?
					raise 'already in default argument'
				end

				in_default = index

				if proto_nesting != 0
					raise 'unexpected nesting level'
				end
			elsif char == ',' && proto_nesting == 0 && !in_default.nil?
				ranges_to_delete.push [ in_default, index ]
				in_default = nil
			end
		end

		if !in_default.nil?
			ranges_to_delete.push [ in_default, args.size ]
			in_default = nil
		end

		correction = 0
		ranges_to_delete.each do |(from, to)|
			args.slice! from - correction, to - from
			correction += to - from
		end

		args = args.split(/,\s*/)
		args.each &:strip!

		@args = args.map { |arg| PrototypeArgument.new arg }
	end
end

class RenderingServerAPISet
	attr_reader :functions
	attr_reader :inner_names

	def initialize(io)
		@functions = []
		@inner_names = InnerNameSet.new

		nesting_level = 0
		rs_at_level = nil

		io.each_line do |line|
			line.strip!

			next if line.empty?

			line.scan /[{}]/ do |match|
				if match == '{'
					nesting_level += 1
				else
					nesting_level -= 1

					if nesting_level < rs_at_level
						rs_at_level = nil
					end
				end
			end

			if line =~ /(?:class|struct|enum)\s*([^\s]+?)\s*\{/ && nesting_level == 2
				@inner_names.add $1
			end

			if line =~ /class RenderingServer/
				rs_at_level = nesting_level
			end

			if nesting_level == rs_at_level
				if line =~ /\Avirtual (.*) = 0;/
					@functions.push FunctionPrototype.new($1)
				end
			end
		end
	end
end

api =
	File.open("../servers/rendering_server.h", "r") do |inf|
		RenderingServerAPISet.new inf
	end

File.open("filament_rendering_server.h", "wb") do |outf|
	outf.write <<EOF
#ifndef FILAMENT_RENDERING_SERVER_H
#define FILAMENT_RENDERING_SERVER_H

#include "servers/rendering_server.h"

#include "filament_rendering_server_backend.h"
#include "threaded_execution_queue.h"

class FilamentDisplayServerContext;

class FilamentRenderingServer final : public RenderingServer, protected ThreadedExecutionQueue<FilamentRenderingServerBackend> {
public:
	explicit FilamentRenderingServer(FilamentDisplayServerContext *context);
	~FilamentRenderingServer() override;

EOF

	api.functions.each do |function|
		outf.puts "\t#{function.return_type} #{function.name}(#{function.args.map { |arg| "#{arg.type} #{arg.name}" }.join(", ") }) #{function.qualifiers} override;"
	end

outf.write <<EOF
};

#endif

EOF

end

File.open("filament_rendering_server_backend.h", "wb") do |outf|
	outf.write <<EOF
#ifndef FILAMENT_RENDERING_SERVER_BACKEND_H
#define FILAMENT_RENDERING_SERVER_BACKEND_H

#include "servers/rendering_server.h"
#include "threaded_execution_queue.h"

class FilamentRenderingServerBackend final : public BaseThreadedExecutionQueueBackend {
public:
	FilamentRenderingServerBackend();
	~FilamentRenderingServerBackend();

	FilamentRenderingServerBackend(const FilamentRenderingServerBackend &other) = delete;
	FilamentRenderingServerBackend &operator =(const FilamentRenderingServerBackend &other) = delete;

EOF

	api.functions.each do |function|
		outf.puts "\t#{api.inner_names.prefix_type function.return_type} #{function.name}(#{function.args.map { |arg| "#{api.inner_names.prefix_type arg.type} #{arg.name}" }.join(", ") }) #{function.qualifiers};"
	end

outf.write <<EOF
};

#endif

EOF
end

File.open("filament_rendering_server.cpp", "wb") do |outf|
	outf.write <<EOF

#include "filament_rendering_server.h"

FilamentRenderingServer::FilamentRenderingServer(FilamentDisplayServerContext *context) {

}

FilamentRenderingServer::~FilamentRenderingServer() = default;

EOF


	api.functions.each do |function|
		outf.puts "#{api.inner_names.prefix_type function.return_type} FilamentRenderingServer::#{function.name}(#{function.args.map { |arg| "#{arg.type} #{arg.name}" }.join(", ") }) #{function.qualifiers} {"

		argpack = [
			"&FilamentRenderingServerBackend::#{function.name}"
		]
		argpack.concat(function.args.map { |arg| arg.name })

		types = [
			function.return_type
		]

		types.concat(function.args.map { |arg| arg.type })

		outf.puts "\treturn execute<#{types.join(", ")}>(#{argpack.join(", ") });"
		outf.puts "};"
		outf.puts ""
	end

end

File.open("filament_rendering_server_backend.cpp", "wb") do |outf|
	outf.write <<EOF

#include "filament_rendering_server_backend.h"
#include <cstdio>

FilamentRenderingServerBackend::FilamentRenderingServerBackend() = default;

FilamentRenderingServerBackend::~FilamentRenderingServerBackend() = default;

EOF


	api.functions.each do |function|
		outf.puts "#{api.inner_names.prefix_type function.return_type} FilamentRenderingServerBackend::#{function.name}(#{function.args.map { |arg| "#{api.inner_names.prefix_type arg.type} #{arg.name}" }.join(", ") }) #{function.qualifiers} {"

		outf.puts "\tprintf(\"FilamentRenderingServerBackend::%s stub!\\n\", #{function.name.inspect});\n"

		if function.return_type != "void"
			outf.puts "\treturn #{api.inner_names.prefix_type function.return_type}();";
		end
		outf.puts "};"
		outf.puts ""
	end

end
