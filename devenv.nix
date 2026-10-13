{
  pkgs,
  # lib,
  # config,
  # inputs,
  ...
}:

{
  # https://devenv.sh/packages/
  packages = [
    pkgs.git
    pkgs.git
  ];

  languages = {
    python = {
      enable = true;
      uv = {
        enable = true;
        sync.enable = true;
      };
      lsp = {
        enable = true;
        package = pkgs.basedpyright;
      };
    };

    c = {
      enable = true;
      debugger = pkgs.gdb;
      lsp = {
        enable = true;
        package = pkgs.clang-tools;
      };
    };
  };

  # https://devenv.sh/basics/
  enterShell = "";
}
