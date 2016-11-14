# mkn-mod.exec

**Runtime command exectution** 


## Usage

    mod:
      - name: mkn-mod.exec
        arg:
            compile:
                echo command


multiple commands

    mod:
      - name: mkn-mod.exec
        arg:
            compile: |
                echo command 1
                echo command 2

**Any command without 0 exit code will result in a phase failure**