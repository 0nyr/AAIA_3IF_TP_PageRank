{
  description = "INSA Lyon, TP 3IF";

  inputs = {
    nixpkgs.url = github:NixOS/nixpkgs/nixos-unstable;
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pythonPackages = pkgs.python311Packages;
      in
      {
        devShells.default = pkgs.mkShell {
          # packages needed to build the environment.
          nativeBuildInputs = with pkgs; [
            # LaTeX
            pkgs.texlive.combined.scheme-full
            pkgs.biber
            pkgs.gnumake
            
            # Adding Java for LTeX spell checker (vscode extension)
            pkgs.openjdk
          ];

          # package needed at build and runtime.
          buildInputs = with pkgs; [
            # C
            pkgs.gcc
            pkgs.gnumake
          ];

          # environment variables.
          shellHook = ''
            export JAVA_HOME=${pkgs.openjdk11}/lib/openjdk
            export JAVA_OPTS="-Xms64m -Xmx512m"
          '';
        };
      }
    );
}
