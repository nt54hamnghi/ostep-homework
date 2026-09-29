{
  pkgs,
  lib,
  config,
  inputs,
  ...
}:

{
  # https://devenv.sh/packages/
  packages = [ pkgs.git ];

  languages.python = {
    enable = true;
    uv = {
      enable = true;
      sync.enable = true;
    };
    lsp = {
      enable = true;
      package = pkgs.ruff;
    };
  };

  # https://devenv.sh/basics/
  enterShell = "";
}
