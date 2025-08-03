import sys
import os

# Add the build directory to Python path so it can find the module
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'build'))

import spz

# Let's see what's available
print("Available in spz module:")
print(dir(spz))

# Check PackOptions attributes
pack_opts = spz.PackOptions()
print("\nPackOptions attributes:")
print(dir(pack_opts))

# Check UnpackOptions attributes  
unpack_opts = spz.UnpackOptions()
print("\nUnpackOptions attributes:")
print(dir(unpack_opts))



def convert_ply_to_spz_with_antialiasing(ply_filename, spz_filename, antialiased=True):
    """
    Convert a PLY file to SPZ format, preserving antialiasing flag
    """
    print(f"Converting {ply_filename} to {spz_filename}")
    print(f"Setting antialiased flag to: {antialiased}")
    
    # Load the PLY file
    unpack_options = spz.UnpackOptions()
    unpack_options.to = spz.CoordinateSystem.RDF  # PLY typically uses RDF
    
    print("Loading PLY file...")
    gaussians = spz.load_splat_from_ply(ply_filename, unpack_options)
    
    print(f"Loaded {gaussians.numPoints} gaussians")
    print(f"SH degree: {gaussians.shDegree}")
    
    # IMPORTANT: Set the antialiased flag
    gaussians.antialiased = antialiased
    print(f"Set antialiased flag to: {gaussians.antialiased}")
    
    # Set up options for saving SPZ
    pack_options = spz.PackOptions()
    pack_options.from_ = spz.CoordinateSystem.RUB  # SPZ uses RUB internally
    
    # Save as SPZ
    print("Saving SPZ file...")
    success = spz.save_spz(gaussians, pack_options, spz_filename)
    
    if success:
        print(f"Successfully saved {spz_filename}")
        
        # Verify the saved file
        verify_options = spz.UnpackOptions()
        verify_options.to = spz.CoordinateSystem.RUB
        verify_gaussians = spz.load_spz(spz_filename, verify_options)
        print(f"Verification: loaded {verify_gaussians.numPoints} gaussians")
        print(f"Verification: antialiased flag is {verify_gaussians.antialiased}")
        
        # Show file sizes
        ply_size = os.path.getsize(ply_filename)
        spz_size = os.path.getsize(spz_filename)
        compression_ratio = ply_size / spz_size
        
        print(f"PLY size: {ply_size:,} bytes")
        print(f"SPZ size: {spz_size:,} bytes")
        print(f"Compression ratio: {compression_ratio:.1f}x")
        
        return True
    else:
        print("Failed to save SPZ file")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python convert_ply_to_spz.py <input.ply> <output.spz> [antialiased]")
        print("  antialiased: true/false (default: true)")
        sys.exit(1)
    
    ply_filename = sys.argv[1]
    spz_filename = sys.argv[2]
    antialiased = True  # Default to True
    
    if len(sys.argv) > 3:
        antialiased = sys.argv[3].lower() == 'true'
    
    if not os.path.exists(ply_filename):
        print(f"Error: PLY file '{ply_filename}' not found")
        sys.exit(1)
    
    convert_ply_to_spz_with_antialiasing(ply_filename, spz_filename, antialiased)