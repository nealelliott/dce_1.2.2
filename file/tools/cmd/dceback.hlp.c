#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/cmd_help.h>

void dceback_host _TAKES((FILE* stream));
void dceback_destfile _TAKES((FILE* stream));
void dceback_stdout _TAKES((FILE* stream));
void dceback_sourcefile _TAKES((FILE* stream));
void dceback_stdin _TAKES((FILE* stream));
void dceback_destdir _TAKES((FILE* stream));
void dceback_help _TAKES((FILE* stream));
void apropos_topic _TAKES((FILE* stream));
void apropos_help _TAKES((FILE* stream));
void dumpcds_host _TAKES((FILE* stream));
void dumpcds_destfile _TAKES((FILE* stream));
void dumpcds_stdout _TAKES((FILE* stream));
void dumpcds_help _TAKES((FILE* stream));
void dumpdfs_host _TAKES((FILE* stream));
void dumpdfs_destfile _TAKES((FILE* stream));
void dumpdfs_stdout _TAKES((FILE* stream));
void dumpdfs_component _TAKES((FILE* stream));
void dumpdfs_help _TAKES((FILE* stream));
void dumpmisc_host _TAKES((FILE* stream));
void dumpmisc_destfile _TAKES((FILE* stream));
void dumpmisc_stdout _TAKES((FILE* stream));
void dumpmisc_help _TAKES((FILE* stream));
void dumpsecurity_host _TAKES((FILE* stream));
void dumpsecurity_destfile _TAKES((FILE* stream));
void dumpsecurity_stdout _TAKES((FILE* stream));
void dumpsecurity_component _TAKES((FILE* stream));
void dumpsecurity_help _TAKES((FILE* stream));
void help_topic _TAKES((FILE* stream));
void help_help _TAKES((FILE* stream));
void restorecds_host _TAKES((FILE* stream));
void restorecds_sourcefile _TAKES((FILE* stream));
void restorecds_stdin _TAKES((FILE* stream));
void restorecds_destdir _TAKES((FILE* stream));
void restorecds_help _TAKES((FILE* stream));
void restoredfs_host _TAKES((FILE* stream));
void restoredfs_sourcefile _TAKES((FILE* stream));
void restoredfs_stdin _TAKES((FILE* stream));
void restoredfs_component _TAKES((FILE* stream));
void restoredfs_destdir _TAKES((FILE* stream));
void restoredfs_help _TAKES((FILE* stream));
void restoremisc_host _TAKES((FILE* stream));
void restoremisc_sourcefile _TAKES((FILE* stream));
void restoremisc_stdin _TAKES((FILE* stream));
void restoremisc_destdir _TAKES((FILE* stream));
void restoremisc_help _TAKES((FILE* stream));
void restoresecurity_host _TAKES((FILE* stream));
void restoresecurity_sourcefile _TAKES((FILE* stream));
void restoresecurity_stdin _TAKES((FILE* stream));
void restoresecurity_component _TAKES((FILE* stream));
void restoresecurity_destdir _TAKES((FILE* stream));
void restoresecurity_help _TAKES((FILE* stream));

cmd_help_entry_t help_array[]={
{"dceback", "-host", "-host <machine_name>", dceback_host},
{"dceback", "-destfile", "-destfile <local_pathname>", dceback_destfile},
{"dceback", "-stdout", "-stdout", dceback_stdout},
{"dceback", "-sourcefile", "-sourcefile <local_pathname>", dceback_sourcefile},
{"dceback", "-stdin", "-stdin", dceback_stdin},
{"dceback", "-destdir", "-destdir <remote_pathname>", dceback_destdir},
{"dceback", "-help", "-help", dceback_help},
{"apropos", "-topic", "-topic <string>", apropos_topic},
{"apropos", "-help", "-help", apropos_help},
{"dumpcds", "-host", "-host <machine_name>", dumpcds_host},
{"dumpcds", "-destfile", "-destfile <local_pathname>", dumpcds_destfile},
{"dumpcds", "-stdout", "-stdout", dumpcds_stdout},
{"dumpcds", "-help", "-help", dumpcds_help},
{"dumpdfs", "-host", "-host <machine_name>", dumpdfs_host},
{"dumpdfs", "-destfile", "-destfile <local_pathname>", dumpdfs_destfile},
{"dumpdfs", "-stdout", "-stdout", dumpdfs_stdout},
{"dumpdfs", "-component", "-component admin, bakdb, config, dfstab, or fldb", dumpdfs_component},
{"dumpdfs", "-help", "-help", dumpdfs_help},
{"dumpmisc", "-host", "-host <machine_name>", dumpmisc_host},
{"dumpmisc", "-destfile", "-destfile <local_pathname>", dumpmisc_destfile},
{"dumpmisc", "-stdout", "-stdout", dumpmisc_stdout},
{"dumpmisc", "-help", "-help", dumpmisc_help},
{"dumpsecurity", "-host", "-host <machine_name>", dumpsecurity_host},
{"dumpsecurity", "-destfile", "-destfile <local_pathname>", dumpsecurity_destfile},
{"dumpsecurity", "-stdout", "-stdout", dumpsecurity_stdout},
{"dumpsecurity", "-component", "-component common or master", dumpsecurity_component},
{"dumpsecurity", "-help", "-help", dumpsecurity_help},
{"help", "-topic", "-topic <string>", help_topic},
{"help", "-help", "-help", help_help},
{"restorecds", "-host", "-host <machine_name>", restorecds_host},
{"restorecds", "-sourcefile", "-sourcefile <local_pathname>", restorecds_sourcefile},
{"restorecds", "-stdin", "-stdin", restorecds_stdin},
{"restorecds", "-destdir", "-destdir <remote_pathname>", restorecds_destdir},
{"restorecds", "-help", "-help", restorecds_help},
{"restoredfs", "-host", "-host <machine_name>", restoredfs_host},
{"restoredfs", "-sourcefile", "-sourcefile <local_pathname>", restoredfs_sourcefile},
{"restoredfs", "-stdin", "-stdin", restoredfs_stdin},
{"restoredfs", "-component", "-component admin, bakdb, config, dfstab, or fldb", restoredfs_component},
{"restoredfs", "-destdir", "-destdir <remote_pathname>", restoredfs_destdir},
{"restoredfs", "-help", "-help", restoredfs_help},
{"restoremisc", "-host", "-host <machine_name>", restoremisc_host},
{"restoremisc", "-sourcefile", "-sourcefile <local_pathname>", restoremisc_sourcefile},
{"restoremisc", "-stdin", "-stdin", restoremisc_stdin},
{"restoremisc", "-destdir", "-destdir <remote_pathname>", restoremisc_destdir},
{"restoremisc", "-help", "-help", restoremisc_help},
{"restoresecurity", "-host", "-host <machine_name>", restoresecurity_host},
{"restoresecurity", "-sourcefile", "-sourcefile <local_pathname>", restoresecurity_sourcefile},
{"restoresecurity", "-stdin", "-stdin", restoresecurity_stdin},
{"restoresecurity", "-component", "-component common or master", restoresecurity_component},
{"restoresecurity", "-destdir", "-destdir <remote_pathname>", restoresecurity_destdir},
{"restoresecurity", "-help", "-help", restoresecurity_help},
{(char *)NULL, (char *)NULL, (char *)NULL, (cmd_print_help_f_t)NULL}
};

void dceback_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine from which data files are to be backed up or to which\n");
fprintf(stream, "data files are to be restored. Specify the machine by its full DCE pathname;\n");
fprintf(stream, "for example, /.../dce.abc.com/hosts/red indicates the machine named\n");
fprintf(stream, "red from the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine on\n");
fprintf(stream, "which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are dumped from or restored to the local machine.\n");
}

void dceback_destfile(stream) FILE* stream;
{
fprintf(stream, "Names the file to which dumped data files are to be written. The pathname of\n");
fprintf(stream, "the specified file must be accessible from the machine on which the command is\n");
fprintf(stream, "issued; the file can be created in a directory on the machine from which the\n");
fprintf(stream, "command is issued or in a directory in the network. A relative pathname is\n");
fprintf(stream, "taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All data files dumped with any one invocation of a dceback command that\n");
fprintf(stream, "includes this option are written to the single file specified with the option.\n");
fprintf(stream, "The dumped data overwrites the contents of an existing file of the same name.\n");
fprintf(stream, "The data is written in tar format, with no compression. You must use\n");
fprintf(stream, "either this option or the -stdout option.\n");
}

void dceback_stdout(stream) FILE* stream;
{
fprintf(stream, "Directs the command to write the data files it dumps to standard output\n");
fprintf(stream, "instead of to a file. The command sends the data to standard output in\n");
fprintf(stream, "tar format, with no compression. Use this option to reduce the amount of\n");
fprintf(stream, "disk space required to store the dumped data by piping the output to the\n");
fprintf(stream, "compress program. You must use either this option or the -destfile\n");
fprintf(stream, "option.\n");
}

void dceback_sourcefile(stream) FILE* stream;
{
fprintf(stream, "Names the file from which previously dumped data files are to be restored. The\n");
fprintf(stream, "pathname of the specified file must be accessible from the machine on which\n");
fprintf(stream, "the command is issued; the file can reside in a directory on the machine from\n");
fprintf(stream, "which the command is issued or in a directory in the network. A relative\n");
fprintf(stream, "pathname is taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All data files restored with any one invocation of a dceback command that\n");
fprintf(stream, "includes this option are read from the single file specified with the option.\n");
fprintf(stream, "You must use either this option or the -stdin option.\n");
}

void dceback_stdin(stream) FILE* stream;
{
fprintf(stream, "Directs the command to read the data files it is to restore from standard\n");
fprintf(stream, "input instead of from a file. Use this option to read output piped to the\n");
fprintf(stream, "command from the zcat program. This option allows you to restore data\n");
fprintf(stream, "files from a compressed tar file created by piping the output of a\n");
fprintf(stream, "dceback command to the compress program. You must use either\n");
fprintf(stream, "this option or the -sourcefile option.\n");
}

void dceback_destdir(stream) FILE* stream;
{
fprintf(stream, "Names an alternate root directory beneath which previously dumped data files\n");
fprintf(stream, "are to be restored. The pathname of the specified directory must be accessible\n");
fprintf(stream, "from the machine indicated with the -host option. It can resolve to a\n");
fprintf(stream, "directory on the indicated machine or to a directory in the network. You must\n");
fprintf(stream, "supply a full pathname; a relative pathname is not acceptable. The specified\n");
fprintf(stream, "directory must already exist.\n");
fprintf(stream, "\n");
fprintf(stream, "By default, each data file is restored to the directory in which it typically\n");
fprintf(stream, "resides (the directory from which it was dumped). You can use this option to\n");
fprintf(stream, "specify an alternate root directory beneath which all files are to be restored.\n");
fprintf(stream, "The command creates the necessary hierarchy of subdirectories if it does not\n");
fprintf(stream, "already exist beneath the specified directory.\n");
}

void dceback_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for the command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void apropos_topic(stream) FILE* stream;
{
fprintf(stream, "Specifies the keyword string for which to search. If it is more than a single\n");
fprintf(stream, "word, surround it with \" \" (double quotes) or other delimiters. Enter all\n");
fprintf(stream, "strings in the case you wish to match (for example, enter the names of all\n");
fprintf(stream, "dceback commands in lowercase letters).\n");
}

void apropos_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void dumpcds_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine from which CDS data files are to be backed up. Specify\n");
fprintf(stream, "the machine by its full DCE pathname; for example,\n");
fprintf(stream, "/.../dce.abc.com/hosts/red indicates the machine named red from\n");
fprintf(stream, "the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option;\n");
fprintf(stream, "all files are backed up from the local machine.\n");
}

void dumpcds_destfile(stream) FILE* stream;
{
fprintf(stream, "Names the file to which dumped CDS data files are to be written. The pathname\n");
fprintf(stream, "of the specified file must be accessible from the machine on which the command\n");
fprintf(stream, "is issued; the file can be created in a directory on the machine from which the\n");
fprintf(stream, "command is issued or in a directory in the network. A relative pathname is\n");
fprintf(stream, "taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All CDS data files dumped with an instance of the dceback dumpcds command\n");
fprintf(stream, "that includes this option are written to the single file specified with the\n");
fprintf(stream, "option. The dumped data overwrites the contents of an existing file of the\n");
fprintf(stream, "same name. The data is written in tar format, with no compression. Use\n");
fprintf(stream, "this option or use the -stdout option.\n");
}

void dumpcds_stdout(stream) FILE* stream;
{
fprintf(stream, "Directs the command to write the CDS data files it dumps to standard output\n");
fprintf(stream, "instead of to a file. The command sends the data to standard output in\n");
fprintf(stream, "tar format, with no compression. Use this option to reduce the amount of\n");
fprintf(stream, "disk space required to store the dumped data by piping the output to the\n");
fprintf(stream, "compress program. Use this option or use the -destfile option.\n");
}

void dumpcds_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void dumpdfs_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine from which DFS data files are to be backed up. Specify\n");
fprintf(stream, "the machine by its full DCE pathname; for example,\n");
fprintf(stream, "/.../dce.abc.com/hosts/red indicates the machine named red from\n");
fprintf(stream, "the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are backed up from the local machine.\n");
}

void dumpdfs_destfile(stream) FILE* stream;
{
fprintf(stream, "Names the file to which dumped DFS data files are to be written. The pathname\n");
fprintf(stream, "of the specified file must be accessible from the machine on which the command\n");
fprintf(stream, "is issued; the file can be created in a directory on the machine from which the\n");
fprintf(stream, "command is issued or in a directory in the network. A relative pathname is\n");
fprintf(stream, "taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All DFS data files dumped with an instance of the dceback dumpdfs command\n");
fprintf(stream, "that includes this option are written to the single file specified with the\n");
fprintf(stream, "option. The dumped data overwrites an existing file of the same name. The data\n");
fprintf(stream, "is written in tar format, with no compression. Use this option or use\n");
fprintf(stream, "the -stdout option.\n");
}

void dumpdfs_stdout(stream) FILE* stream;
{
fprintf(stream, "Directs the command to write the DFS data files it dumps to standard output\n");
fprintf(stream, "instead of to a file. The command sends the data to standard output in\n");
fprintf(stream, "tar format, with no compression. Use this option to reduce the amount of\n");
fprintf(stream, "disk space required to store the dumped data by piping the output to the\n");
fprintf(stream, "compress program. Use this option or use the -destfile option.\n");
}

void dumpdfs_component(stream) FILE* stream;
{
fprintf(stream, "Specifies the DFS data files to be backed up. Specify one or more of the\n");
fprintf(stream, "following arguments:\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin\n");
fprintf(stream, "Directs the command to dump any of the following administrative lists that it\n");
fprintf(stream, "finds in the <dcelocal>/var/dfs directory. Administrative lists define\n");
fprintf(stream, "the principals and groups that can manipulate DFS server processes on a\n");
fprintf(stream, "machine; all administrative lists are host independent.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.bak\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate the DFS Backup\n");
fprintf(stream, "System. It resides on each machine configured as a Backup Server, each of which\n");
fprintf(stream, "runs the Backup Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.bos\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate the Basic\n");
fprintf(stream, "OverSeer (BOS) Server on the machine. It resides on each machine configured as\n");
fprintf(stream, "a DFS server, each of which runs the BOS Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.fl\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate information in\n");
fprintf(stream, "the DFS Fileset Location Database (FLDB). It resides on each machine configured\n");
fprintf(stream, "as a Fileset Location Server, each of which runs the Fileset Location (FL)\n");
fprintf(stream, "Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.ft\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate filesets on\n");
fprintf(stream, "the machine. It resides on each machine configured as a File Server, each of\n");
fprintf(stream, "which runs the Fileset Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.up\n");
fprintf(stream, "A file that lists the server principals that can receive, via the Update\n");
fprintf(stream, "Server, updated copies of configuration files that reside on the machine. It\n");
fprintf(stream, "resides on all machines configured as System Control machines, each of which\n");
fprintf(stream, "runs the server portion of the Update Server.\n");
fprintf(stream, "\n");
fprintf(stream, "Before backing up the administrative lists from a machine, stop the BOS Server\n");
fprintf(stream, "process on that machine. Stopping the BOS Server process ensures that no one\n");
fprintf(stream, "is modifying an administrative list when it is backed up. When the command is\n");
fprintf(stream, "finished executing, restart the BOS Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ bakdb\n");
fprintf(stream, "Directs the command to dump the bkdb.DB0 and bkdb.DBSYS1 files from\n");
fprintf(stream, "the <dcelocal>/var/dfs/backup directory. These files compose the DFS\n");
fprintf(stream, "Backup Database. They reside only on machines configured as Backup Servers.\n");
fprintf(stream, "They are identical on all Backup Servers.\n");
fprintf(stream, "\n");
fprintf(stream, "Before backing up the Backup Database from a machine, stop the Backup Server\n");
fprintf(stream, "process on that machine. Stopping the Backup Server process ensures that no\n");
fprintf(stream, "one is modifying information in the copy of the Backup Database on the machine\n");
fprintf(stream, "when it is backed up. When the command is finished executing, restart the\n");
fprintf(stream, "Backup Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ config\n");
fprintf(stream, "Directs the command to dump any of the following configuration files that it\n");
fprintf(stream, "finds on the indicated machine:\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/var/dfs/BosConfig\n");
fprintf(stream, "A file that defines the server processes monitored by the BOS Server, as well\n");
fprintf(stream, "as additional parameters associated with the BOS Server and the processes it\n");
fprintf(stream, "monitors. It resides on each machine configured as a DFS server, each of which\n");
fprintf(stream, "runs the BOS Server process. It is host independent for machines that run the\n");
fprintf(stream, "same processes (for example, it is typically consistent between File Server\n");
fprintf(stream, "machines).\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/etc/CacheInfo\n");
fprintf(stream, "A file that specifies the initial configuration of the DFS Cache Manager. It\n");
fprintf(stream, "resides on each machine configured as a DFS client. It contains a single line\n");
fprintf(stream, "that consists of three colon-separated fields specifying the following\n");
fprintf(stream, "information: the directory on the local disk where the Cache Manager mounts\n");
fprintf(stream, "the global namespace (/...); the directory on the local disk that is to\n");
fprintf(stream, "serve as the cache directory for a disk cache; and the size of the local cache\n");
fprintf(stream, "in 1-kilobyte blocks. The file is host independent for machines that have the\n");
fprintf(stream, "same Cache Manager configuration; it has the following format:\n");
fprintf(stream, "   \n");
fprintf(stream, "   /...:<cache_directory>:<cache_size>\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/etc/rc.dfs.conf\n");
fprintf(stream, "A file that stores information about the DFS processes configured on a machine.\n");
fprintf(stream, "It resides on each machine configured as a DFS client or any type of DFS\n");
fprintf(stream, "server. The information it contains is used by the DFS initialization file\n");
fprintf(stream, "(/etc/init.d/dfs) to start and stop DFS processes on the machine.\n");
fprintf(stream, "Although the rc.dfs.conf file stores information about specific DFS\n");
fprintf(stream, "processes, it is host independent for machines that run the same DFS processes.\n");
fprintf(stream, "\n");
fprintf(stream, "Before backing up the BosConfig file from a machine, stop the BOS Server\n");
fprintf(stream, "process on that machine. Stopping the BOS Server process ensures that no one\n");
fprintf(stream, "is modifying the BosConfig file when it is backed up. When the command\n");
fprintf(stream, "is finished executing, restart the BOS Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "Before backing up the CacheInfo or rc.dfs.conf file from a machine,\n");
fprintf(stream, "ensure that no one is editing either of the files. If one of the files is being\n");
fprintf(stream, "edited when you issue this command, you may not dump the most-recent version of\n");
fprintf(stream, "the file, or you may dump a version of the file that is in an inconsistent\n");
fprintf(stream, "state.\n");
fprintf(stream, "\n");
fprintf(stream, "+ dfstab\n");
fprintf(stream, "Directs the command to dump the <dcelocal>/var/dfs/dfstab file. This\n");
fprintf(stream, "file specifies the aggregates to be exported from the machine to the DCE\n");
fprintf(stream, "namespace. It resides on each machine configured as a File Server. Although\n");
fprintf(stream, "it lists the names of specific devices, the file is host independent for File\n");
fprintf(stream, "Server machines that export devices of the same name.\n");
fprintf(stream, "\n");
fprintf(stream, "Ensure that no one is editing the dfstab file when you issue this\n");
fprintf(stream, "command. If the file is being edited when you issue this command, you may not\n");
fprintf(stream, "dump the most-recent version of the file, or you may dump a version of the\n");
fprintf(stream, "file that is in an inconsistent state.\n");
fprintf(stream, "\n");
fprintf(stream, "+ fldb\n");
fprintf(stream, "Directs the command to dump the fldb.DB0 and fldb.DBSYS1 files from\n");
fprintf(stream, "the <dcelocal>/var/dfs directory. These files compose the FLDB. They\n");
fprintf(stream, "reside only on machines configured as Fileset Location Servers. They are\n");
fprintf(stream, "identical on all Fileset Location Servers.\n");
fprintf(stream, "\n");
fprintf(stream, "Before backing up the FLDB from a machine, stop the FL Server process on that\n");
fprintf(stream, "machine. Stopping the FL Server process ensures that no one is modifying\n");
fprintf(stream, "information in the copy of the FLDB on the machine when it is backed up. When\n");
fprintf(stream, "the command is finished executing, restart the FL Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "Omit this option to back up all of the DFS data files on the machine. Omitting\n");
fprintf(stream, "this option is equivalent to specifying it with all of its acceptable\n");
fprintf(stream, "arguments.\n");
}

void dumpdfs_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void dumpmisc_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine from which miscellaneous DCE data files are to be backed\n");
fprintf(stream, "up. Specify the machine by its full DCE pathname; for example,\n");
fprintf(stream, "/.../dce.abc.com/hosts/red indicates the machine named red from\n");
fprintf(stream, "the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are backed up from the local machine.\n");
}

void dumpmisc_destfile(stream) FILE* stream;
{
fprintf(stream, "Names the file to which dumped miscellaneous DCE data files are to be written.\n");
fprintf(stream, "The pathname of the specified file must be accessible from the machine on which\n");
fprintf(stream, "the command is issued; the file can be created in a directory on the machine\n");
fprintf(stream, "from which the command is issued or in a directory in the network. A relative\n");
fprintf(stream, "pathname is taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All miscellaneous DCE data files dumped with an instance of the dceback\n");
fprintf(stream, "dumpmisc command that includes this option are written to the single file\n");
fprintf(stream, "specified with the option. The dumped data overwrites an existing file of\n");
fprintf(stream, "the same name. The data is written in tar format, with no compression.\n");
fprintf(stream, "Use this option or use the -stdout option.\n");
}

void dumpmisc_stdout(stream) FILE* stream;
{
fprintf(stream, "Directs the command to write the miscellaneous DCE data files it dumps to\n");
fprintf(stream, "standard output instead of to a file. The command sends the data to standard\n");
fprintf(stream, "output in tar format, with no compression. Use this option to reduce the\n");
fprintf(stream, "amount of disk space required to store the dumped data by piping the output\n");
fprintf(stream, "to the compress program. Use this option or use the -destfile\n");
fprintf(stream, "option.\n");
}

void dumpmisc_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void dumpsecurity_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine from which Security Service data files are to be backed\n");
fprintf(stream, "up. Specify the machine by its full DCE pathname; for example,\n");
fprintf(stream, "/.../dce.abc.com/hosts/red indicates the machine named red from\n");
fprintf(stream, "the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are backed up from the local machine.\n");
}

void dumpsecurity_destfile(stream) FILE* stream;
{
fprintf(stream, "Names the file to which dumped Security Service data files are to be written.\n");
fprintf(stream, "The pathname of the specified file must be accessible from the machine on which\n");
fprintf(stream, "the command is issued; the file can be created in a directory on the machine\n");
fprintf(stream, "from which the command is issued or in a directory in the network. A relative\n");
fprintf(stream, "pathname is taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All Security Service data files dumped with an instance of the dceback\n");
fprintf(stream, "dumpsecurity command that includes this option are written to the single\n");
fprintf(stream, "file specified with the option. The dumped data overwrites an existing file\n");
fprintf(stream, "of the same name. The data is written in tar format, with no\n");
fprintf(stream, "compression. Use this option or use the -stdout option.\n");
}

void dumpsecurity_stdout(stream) FILE* stream;
{
fprintf(stream, "Directs the command to write the Security Service data files it dumps to\n");
fprintf(stream, "standard output instead of to a file. The command sends the data to standard\n");
fprintf(stream, "output in tar format, with no compression. Use this option to reduce\n");
fprintf(stream, "the amount of disk space required to store the dumped data by piping the\n");
fprintf(stream, "output to the compress program. Use this option or use the\n");
fprintf(stream, "-destfile option.\n");
}

void dumpsecurity_component(stream) FILE* stream;
{
fprintf(stream, "Specifies the Security Service data files to be backed up. Specify one or both\n");
fprintf(stream, "of the following arguments:\n");
fprintf(stream, "\n");
fprintf(stream, "+ common\n");
fprintf(stream, "Directs the command to dump Security Service data files common to every machine\n");
fprintf(stream, "configured as a DCE client. The command dumps the following files:\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/var/security/lrgy_data\n");
fprintf(stream, "A host-specific file that contains the local login cache. The file contains\n");
fprintf(stream, "an entry for each user who effectively authenticates to DCE on the machine.\n");
fprintf(stream, "It never contains more than a specified number of entries, typically 25. Each\n");
fprintf(stream, "entry contains account information that can be used to provide local\n");
fprintf(stream, "authentication in the event that a Security Server is not available. With the\n");
fprintf(stream, "lrgy_tgts file, this file constitutes a machine's local registry; the\n");
fprintf(stream, "two files may not exist on every machine.\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/var/security/lrgy_tgts\n");
fprintf(stream, "A host-specific file that contains a valid ticket-granting ticket (TGT) for\n");
fprintf(stream, "each entry in the local lrgy_data file. The information in the TGT allows\n");
fprintf(stream, "a user with an entry in the local login cache to authenticate to the local\n");
fprintf(stream, "machine in the event that a Security Server is not available. With the\n");
fprintf(stream, "lrgy_data file, this file composes a machine's local registry; the two\n");
fprintf(stream, "files may not exist on every machine.\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/etc/security/pe_site\n");
fprintf(stream, "A host-independent file that contains information necessary for the machine to\n");
fprintf(stream, "contact the cell's master Security Server. For each protocol sequence supported\n");
fprintf(stream, "by the cell's master Security Server, the file contains a line that lists the\n");
fprintf(stream, "name of the cell to which the machine belongs and a string binding that\n");
fprintf(stream, "includes the master Security Server's protocol sequence, as follows:\n");
fprintf(stream, "   \n");
fprintf(stream, "   /.../<cell_name> <UUID>@<prot_seq>:<IP_address>[<port_number>]\n");
fprintf(stream, "\n");
fprintf(stream, "A copy of this file resides on every machine configured as a DCE client.\n");
fprintf(stream, "\n");
fprintf(stream, "+ /krb5/krb.conf\n");
fprintf(stream, "A host-independent Kerberos configuration file that contains two lines: the\n");
fprintf(stream, "first line lists the name of the cell to which the machine belongs, and the\n");
fprintf(stream, "second line lists the name of the cell to which the machine belongs, followed\n");
fprintf(stream, "by the name of the cell's master Security Server. The file has the following\n");
fprintf(stream, "format:\n");
fprintf(stream, "   \n");
fprintf(stream, "   <cell_name>\n");
fprintf(stream, "   <cell_name> <Security_Server_hostname>\n");
fprintf(stream, "\n");
fprintf(stream, "A copy of this file resides on every machine configured as a DCE client.\n");
fprintf(stream, "\n");
fprintf(stream, "+ /krb5/v5srvtab\n");
fprintf(stream, "A host-specific keytab file that contains the encryption keys (and their\n");
fprintf(stream, "version numbers) for server processes that run on the local machine. An\n");
fprintf(stream, "encryption key enables processes to communicate securely by using the key to\n");
fprintf(stream, "encrypt and decrypt messages.\n");
fprintf(stream, "\n");
fprintf(stream, "You should ensure that no one is editing one of these files when you issue this\n");
fprintf(stream, "command. If a file to be backed up is being edited when you issue this command,\n");
fprintf(stream, "you may not dump the most-recent version of the file, or you may dump a version\n");
fprintf(stream, "of the file that is in an inconsistent state. Because any DCE process running\n");
fprintf(stream, "on a machine can modify the keytab file on that machine, you may want to stop\n");
fprintf(stream, "all DCE processes on a machine before backing up common Security Service data\n");
fprintf(stream, "files, restarting the stopped processes once the files have been backed up.\n");
fprintf(stream, "\n");
fprintf(stream, "+ master\n");
fprintf(stream, "Directs the command to dump the registry database and the master key for\n");
fprintf(stream, "the database. The command dumps the following files from the\n");
fprintf(stream, "<dcelocal>/var/security directory:\n");
fprintf(stream, "\n");
fprintf(stream, "+ .mkey\n");
fprintf(stream, "A file that contains the master key used to encrypt all passwords in the\n");
fprintf(stream, "registry database. Because the file contains the master key for the registry\n");
fprintf(stream, "database, the database is useless without it. The file itself is host\n");
fprintf(stream, "independent.\n");
fprintf(stream, "\n");
fprintf(stream, "+ rgy_data\n");
fprintf(stream, "A directory that contains the files that house the registry database. The\n");
fprintf(stream, "directory and its contents are host independent.\n");
fprintf(stream, "\n");
fprintf(stream, "Use the master argument to dump these files only from the machine\n");
fprintf(stream, "configured as the cell's master Security Server. Before backing up these files,\n");
fprintf(stream, "use the sec_admin command to ensure that the master copy of the registry\n");
fprintf(stream, "database is the default replica and to place the master copy of the database\n");
fprintf(stream, "in the maintenance state. These steps cause the master Security Server to save\n");
fprintf(stream, "its copy of the database to disk and refuse all updates. Once the files have\n");
fprintf(stream, "been backed up, use the sec_admin command to return the registry\n");
fprintf(stream, "database to the service state. This step causes the master Security Server to\n");
fprintf(stream, "resume accepting updates.\n");
fprintf(stream, "\n");
fprintf(stream, "Each machine configured as a slave Security Server houses a replica of the\n");
fprintf(stream, "registry database and a .mkey file, which is unique for its copy of the\n");
fprintf(stream, "registry database (each .mkey file is unique for the copy of the database\n");
fprintf(stream, "that resides on its machine). Nonetheless, use the master argument to\n");
fprintf(stream, "dump these files only from the machine configured as the master Security\n");
fprintf(stream, "Server.\n");
fprintf(stream, "\n");
fprintf(stream, "Omit this option to back up all of the Security Service data files on the\n");
fprintf(stream, "machine. Omitting this option is equivalent to specifying it with both of its\n");
fprintf(stream, "acceptable arguments. Omit this option only if the machine from which files\n");
fprintf(stream, "are to be backed up is configured as the master Security Server for the cell.\n");
}

void dumpsecurity_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void help_topic(stream) FILE* stream;
{
fprintf(stream, "Specifies each command whose syntax is to be displayed. Provide only the\n");
fprintf(stream, "second part of the command name (for example, dumpcds, not dceback\n");
fprintf(stream, "dumpcds). Omit this option to display short descriptions of all\n");
fprintf(stream, "dceback commands.\n");
}

void help_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void restorecds_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine to which CDS data files are to be restored. The data\n");
fprintf(stream, "files should be restored to the machine from which they were dumped with\n");
fprintf(stream, "the dceback dumpcds command. Specify the machine by its full DCE\n");
fprintf(stream, "pathname; for example, /.../dce.abc.com/hosts/red indicates the machine\n");
fprintf(stream, "named red from the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are restored to the local machine.\n");
}

void restorecds_sourcefile(stream) FILE* stream;
{
fprintf(stream, "Names the file from which previously dumped CDS data files are to be restored.\n");
fprintf(stream, "The pathname of the specified file must be accessible from the machine on which\n");
fprintf(stream, "the command is issued; the file can reside in a directory on the machine from\n");
fprintf(stream, "which the command is issued or in a directory in the network. A relative\n");
fprintf(stream, "pathname is taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All data files restored with an instance of the dceback restorecds\n");
fprintf(stream, "command that includes this option are read from the single file specified\n");
fprintf(stream, "with the option. Use this option or use the -stdin option.\n");
}

void restorecds_stdin(stream) FILE* stream;
{
fprintf(stream, "Directs the command to read the CDS data files it is to restore from standard\n");
fprintf(stream, "input instead of from a file. Use this option to read output piped to the\n");
fprintf(stream, "dceback restorecds command from the zcat program. This option\n");
fprintf(stream, "allows you to restore data files from a compressed tar file created by\n");
fprintf(stream, "piping the output of the dceback dumpcds command to the compress\n");
fprintf(stream, "program. Use this option or use the -sourcefile option.\n");
}

void restorecds_destdir(stream) FILE* stream;
{
fprintf(stream, "Names an alternate root directory beneath which previously dumped CDS data\n");
fprintf(stream, "files are to be restored. The pathname of the specified directory must be\n");
fprintf(stream, "accessible from the machine indicated with the -host option. It can\n");
fprintf(stream, "resolve to a directory on the indicated machine or to a directory in the\n");
fprintf(stream, "network. You must supply a full pathname; a relative pathname is not\n");
fprintf(stream, "acceptable. The specified directory must already exist.\n");
fprintf(stream, "\n");
fprintf(stream, "By default, each data file is restored to the directory in which it typically\n");
fprintf(stream, "resides (the directory from which it was dumped). You can use this option to\n");
fprintf(stream, "specify an alternate root directory beneath which all files are to be restored.\n");
fprintf(stream, "The command creates the necessary hierarchy of subdirectories if it does not\n");
fprintf(stream, "already exist beneath the specified directory.\n");
}

void restorecds_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void restoredfs_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine to which DFS data files are to be restored. Specify\n");
fprintf(stream, "the machine by its full DCE pathname; for example,\n");
fprintf(stream, "/.../dce.abc.com/hosts/red indicates the machine named red\n");
fprintf(stream, "from the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are restored to the local machine.\n");
}

void restoredfs_sourcefile(stream) FILE* stream;
{
fprintf(stream, "Names the file from which previously dumped DFS data files are to be restored.\n");
fprintf(stream, "The pathname of the specified file must be accessible from the machine on which\n");
fprintf(stream, "the command is issued; the file can reside in a directory on the machine from\n");
fprintf(stream, "which the command is issued or in a directory in the network. A relative\n");
fprintf(stream, "pathname is taken relative to the directory from which the command is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All data files restored with an instance of the dceback restoredfs\n");
fprintf(stream, "command that includes this option are read from the single file specified\n");
fprintf(stream, "with the option. Use this option or use the -stdin option.\n");
}

void restoredfs_stdin(stream) FILE* stream;
{
fprintf(stream, "Directs the command to read the DFS data files it is to restore from standard\n");
fprintf(stream, "input instead of from a file. Use this option to read output piped to the\n");
fprintf(stream, "dceback restoredfs command from the zcat program. This option\n");
fprintf(stream, "allows you to restore data files from a compressed tar file created by\n");
fprintf(stream, "piping the output of the dceback dumpdfs command to the compress\n");
fprintf(stream, "program. Use this option or use the -sourcefile option.\n");
}

void restoredfs_component(stream) FILE* stream;
{
fprintf(stream, "Specifies the DFS data files to be restored from the indicated source file.\n");
fprintf(stream, "Only those files previously dumped to the source file with the dceback\n");
fprintf(stream, "dumpdfs command can be restored. Specify one or more of the following\n");
fprintf(stream, "arguments:\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin\n");
fprintf(stream, "Directs the command to restore any administrative lists that it finds in the\n");
fprintf(stream, "source file to the <dcelocal>/var/dfs directory. Administrative lists\n");
fprintf(stream, "define the principals and groups that can manipulate DFS server processes on\n");
fprintf(stream, "a machine; all administrative lists are host independent. The source file can\n");
fprintf(stream, "contain the following administrative lists:\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.bak\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate the DFS Backup\n");
fprintf(stream, "System. It resides on each machine configured as a Backup Server, each of which\n");
fprintf(stream, "runs the Backup Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.bos\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate the Basic\n");
fprintf(stream, "OverSeer (BOS) Server on the machine. It resides on each machine configured as\n");
fprintf(stream, "a DFS server, each of which runs the BOS Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.fl\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate information in\n");
fprintf(stream, "the DFS Fileset Location Database (FLDB). It resides on each machine configured\n");
fprintf(stream, "as a Fileset Location Server, each of which runs the Fileset Location (FL)\n");
fprintf(stream, "Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.ft\n");
fprintf(stream, "A file that lists the principals and groups that can manipulate filesets on\n");
fprintf(stream, "the machine. It resides on each machine configured as a File Server, each of\n");
fprintf(stream, "which runs the Fileset Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ admin.up\n");
fprintf(stream, "A file that lists the server principals that can receive, via the Update\n");
fprintf(stream, "Server, updated copies of configuration files that reside on the machine. It\n");
fprintf(stream, "resides on all machines configured as System Control machines, each of which\n");
fprintf(stream, "runs the server portion of the Update Server.\n");
fprintf(stream, "\n");
fprintf(stream, "Before restoring administrative lists to a machine, stop the BOS Server\n");
fprintf(stream, "process on that machine. Stopping the BOS Server process ensures that no one\n");
fprintf(stream, "is modifying an administrative list when it is restored. When the command is\n");
fprintf(stream, "finished executing, restart the BOS Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "+ bakdb\n");
fprintf(stream, "Directs the command to restore the bkdb.DB0 and bkdb.DBSYS1 files\n");
fprintf(stream, "to the <dcelocal>/var/dfs/backup directory. These files compose the DFS\n");
fprintf(stream, "Backup Database. They reside only on machines configured as Backup Servers.\n");
fprintf(stream, "They are identical on all Backup Servers.\n");
fprintf(stream, "\n");
fprintf(stream, "Before restoring the Backup Database to a machine configured as a Backup\n");
fprintf(stream, "Server, stop the Backup Server process on each machine configured as a Backup\n");
fprintf(stream, "Server. Then restore the Backup Database to each Backup Server machine.\n");
fprintf(stream, "Stopping all Backup Server processes and restoring the Backup Database to all\n");
fprintf(stream, "Backup Servers ensures that Ubik, the DFS database synchronization mechanism,\n");
fprintf(stream, "does not overwrite the restored version of the database with the existing\n");
fprintf(stream, "version from another Backup Server. Once the database is restored to all\n");
fprintf(stream, "Backup Servers, restart all Backup Server processes.\n");
fprintf(stream, "\n");
fprintf(stream, "It is not necessary to stop and restart Backup Server processes to restore the\n");
fprintf(stream, "database to a machine that is not configured as a Backup Server. However, a\n");
fprintf(stream, "version of the database restored to such a machine is not readily accessible to\n");
fprintf(stream, "other machines in the cell; additional configuration is required to prepare the\n");
fprintf(stream, "machine for use as a Backup Server.\n");
fprintf(stream, "\n");
fprintf(stream, "+ config\n");
fprintf(stream, "Directs the command to restore any of the following configuration files that\n");
fprintf(stream, "it finds in the source file:\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/var/dfs/BosConfig\n");
fprintf(stream, "A file that defines the server processes monitored by the BOS Server, as well\n");
fprintf(stream, "as additional parameters associated with the BOS Server and the processes it\n");
fprintf(stream, "monitors. It resides on each machine configured as a DFS server, each of which\n");
fprintf(stream, "runs the BOS Server process. It is host independent for machines that run the\n");
fprintf(stream, "same processes (for example, it is typically consistent between File Server\n");
fprintf(stream, "machines).\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/etc/CacheInfo\n");
fprintf(stream, "A file that specifies the initial configuration of the DFS Cache Manager. It\n");
fprintf(stream, "resides on each machine configured as a DFS client. It contains a single line\n");
fprintf(stream, "that consists of three colon-separated fields specifying the following\n");
fprintf(stream, "information: the directory on the local disk where the Cache Manager mounts\n");
fprintf(stream, "the global namespace (/...); the directory on the local disk that is to\n");
fprintf(stream, "serve as the cache directory for a disk cache; and the size of the local cache\n");
fprintf(stream, "in 1-kilobyte blocks. The file is host independent for machines that have the\n");
fprintf(stream, "same Cache Manager configuration; it has the following format:\n");
fprintf(stream, "   \n");
fprintf(stream, "   /...:<cache_directory>:<cache_size>\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/etc/rc.dfs.conf\n");
fprintf(stream, "A file that stores information about the DFS processes configured on a machine.\n");
fprintf(stream, "It resides on each machine configured as a DFS client or any type of DFS\n");
fprintf(stream, "server. The information it contains is used by the DFS initialization file\n");
fprintf(stream, "(/etc/init.d/dfs) to start and stop DFS processes on the machine.\n");
fprintf(stream, "Although the rc.dfs.conf file stores information about specific DFS\n");
fprintf(stream, "processes, it is host independent for machines that run the same DFS processes.\n");
fprintf(stream, "\n");
fprintf(stream, "Before restoring the BosConfig file to a machine, stop the BOS Server\n");
fprintf(stream, "process on that machine. Stopping the BOS Server process ensures that no one\n");
fprintf(stream, "is modifying the BosConfig file when it is restored. When the command is\n");
fprintf(stream, "finished executing, restart the BOS Server process.\n");
fprintf(stream, "\n");
fprintf(stream, "Before restoring the CacheInfo or rc.dfs.conf file to a machine,\n");
fprintf(stream, "ensure that no one is editing either of the files. If one of the files is\n");
fprintf(stream, "being edited when you issue this command, the restored version of the file\n");
fprintf(stream, "may be overwritten when the version being edited is saved.\n");
fprintf(stream, "\n");
fprintf(stream, "+ dfstab\n");
fprintf(stream, "Directs the command to restore the <dcelocal>/var/dfs/dfstab file.\n");
fprintf(stream, "This file specifies the aggregates to be exported from the machine to the DCE\n");
fprintf(stream, "namespace. It resides on each machine configured as a File Server. Although\n");
fprintf(stream, "it lists the names of specific devices, the file is host independent for File\n");
fprintf(stream, "Server machines that export devices of the same name.\n");
fprintf(stream, "\n");
fprintf(stream, "Ensure that no one is editing the dfstab file when you issue this\n");
fprintf(stream, "command. If the file is being edited when you issue this command, the restored\n");
fprintf(stream, "version of the file may be overwritten when the version being edited is saved.\n");
fprintf(stream, "\n");
fprintf(stream, "+ fldb\n");
fprintf(stream, "Directs the command to restore the fldb.DB0 and fldb.DBSYS1 files\n");
fprintf(stream, "to the <dcelocal>/var/dfs directory. These files compose the FLDB. They\n");
fprintf(stream, "reside only on machines configured as Fileset Location Servers. They are\n");
fprintf(stream, "identical on all Fileset Location Servers.\n");
fprintf(stream, "\n");
fprintf(stream, "Before restoring the FLDB to a machine configured as a Fileset Location Server,\n");
fprintf(stream, "stop the FL Server process on each machine configured as a Fileset Location\n");
fprintf(stream, "Server. Then restore the FLDB to each Fileset Location Server machine. Stopping\n");
fprintf(stream, "all FL Server processes and restoring the FLDB to all Fileset Location Servers\n");
fprintf(stream, "ensures that Ubik, the DFS database synchronization mechanism, does not\n");
fprintf(stream, "overwrite the restored version of the database with the existing version from\n");
fprintf(stream, "another Fileset Location Server.\n");
fprintf(stream, "\n");
fprintf(stream, "Once the database is restored to all Fileset Location Servers, restart all FL\n");
fprintf(stream, "Server processes. After the FL Server processes are restarted, issue the cm\n");
fprintf(stream, "checkfilesets command on each machine configured as a DFS client; the\n");
fprintf(stream, "command forces clients to update their knowledge of fileset information from\n");
fprintf(stream, "the FLDB.\n");
fprintf(stream, "\n");
fprintf(stream, "It is not necessary to stop and restart FL Server processes to restore the\n");
fprintf(stream, "database to a machine that is not configured as Fileset Location Server.\n");
fprintf(stream, "However, a version of the database restored to such a machine is not readily\n");
fprintf(stream, "accessible to other machines in the cell; additional configuration is required\n");
fprintf(stream, "to prepare the machine for use as a Fileset Location Server.\n");
fprintf(stream, "\n");
fprintf(stream, "Omit this option to restore all of the DFS data files contained in the source\n");
fprintf(stream, "file to the machine. Omitting this option is equivalent to specifying it with\n");
fprintf(stream, "all of its acceptable arguments.\n");
}

void restoredfs_destdir(stream) FILE* stream;
{
fprintf(stream, "Names an alternate root directory beneath which previously dumped DFS data\n");
fprintf(stream, "files are to be restored. The pathname of the specified directory must be\n");
fprintf(stream, "accessible from the machine indicated with the -host option. It can\n");
fprintf(stream, "resolve to a directory on the indicated machine or to a directory in the\n");
fprintf(stream, "network. You must supply a full pathname; a relative pathname is not\n");
fprintf(stream, "acceptable. The specified directory must already exist.\n");
fprintf(stream, "\n");
fprintf(stream, "By default, each data file is restored to the directory in which it typically\n");
fprintf(stream, "resides (the directory from which it was dumped). You can use this option to\n");
fprintf(stream, "specify an alternate root directory beneath which all files are to be restored.\n");
fprintf(stream, "The command creates the necessary hierarchy of subdirectories if it does not\n");
fprintf(stream, "already exist beneath the specified directory.\n");
}

void restoredfs_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void restoremisc_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine to which miscellaneous DCE data files are to be\n");
fprintf(stream, "restored. The data files should be restored to the machine from which they\n");
fprintf(stream, "were dumped with the dceback dumpmisc command. Specify the machine\n");
fprintf(stream, "by its full DCE pathname; for example, /.../dce.abc.com/hosts/red\n");
fprintf(stream, "indicates the machine named red from the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are restored to the local machine.\n");
}

void restoremisc_sourcefile(stream) FILE* stream;
{
fprintf(stream, "Names the file from which previously dumped miscellaneous DCE data files are\n");
fprintf(stream, "to be restored. The pathname of the specified file must be accessible from the\n");
fprintf(stream, "machine on which the command is issued; the file can reside in a directory on\n");
fprintf(stream, "the machine from which the command is issued or in a directory in the network.\n");
fprintf(stream, "A relative pathname is taken relative to the directory from which the command\n");
fprintf(stream, "is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All data files restored with an instance of the dceback restoremisc\n");
fprintf(stream, "command that includes this option are read from the single file specified\n");
fprintf(stream, "with the option. Use this option or use the -stdin option.\n");
}

void restoremisc_stdin(stream) FILE* stream;
{
fprintf(stream, "Directs the command to read the miscellaneous DCE data files it is to restore\n");
fprintf(stream, "from standard input instead of from a file. Use this option to read output\n");
fprintf(stream, "piped to the dceback restoremisc command from the zcat program.\n");
fprintf(stream, "This option allows you to restore data files from a compressed tar file\n");
fprintf(stream, "created by piping the output of the dceback dumpmisc command to the\n");
fprintf(stream, "compress program. Use this option or use the -sourcefile option.\n");
}

void restoremisc_destdir(stream) FILE* stream;
{
fprintf(stream, "Names an alternate root directory beneath which previously dumped miscellaneous\n");
fprintf(stream, "DCE data files are to be restored. The pathname of the specified directory must\n");
fprintf(stream, "be accessible from the machine indicated with the -host option. It can\n");
fprintf(stream, "resolve to a directory on the indicated machine or to a directory in the\n");
fprintf(stream, "network. You must supply a full pathname; a relative pathname is not\n");
fprintf(stream, "acceptable. The specified directory must already exist.\n");
fprintf(stream, "\n");
fprintf(stream, "By default, each data file is restored to the directory in which it typically\n");
fprintf(stream, "resides (the directory from which it was dumped). You can use this option to\n");
fprintf(stream, "specify an alternate root directory beneath which all files are to be restored.\n");
fprintf(stream, "The command creates the necessary hierarchy of subdirectories if it does not\n");
fprintf(stream, "already exist beneath the specified directory.\n");
}

void restoremisc_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

void restoresecurity_host(stream) FILE* stream;
{
fprintf(stream, "Specifies the machine to which Security Service data files are to be restored.\n");
fprintf(stream, "The data files should be restored to the machine from which they were dumped\n");
fprintf(stream, "with the dceback dumpsecurity command. Specify the machine by its full\n");
fprintf(stream, "DCE pathname; for example, /.../dce.abc.com/hosts/red indicates the\n");
fprintf(stream, "machine named red from the cell named dce.abc.com.\n");
fprintf(stream, "\n");
fprintf(stream, "Currently, the only legal argument for this option is the literal value\n");
fprintf(stream, "local. This value directs the command to use the name of the machine\n");
fprintf(stream, "on which the command is issued as the argument for the -host option; all\n");
fprintf(stream, "files are restored to the local machine.\n");
}

void restoresecurity_sourcefile(stream) FILE* stream;
{
fprintf(stream, "Names the file from which previously dumped Security Service data files are to\n");
fprintf(stream, "be restored. The pathname of the specified file must be accessible from the\n");
fprintf(stream, "machine on which the command is issued; the file can reside in a directory on\n");
fprintf(stream, "the machine from which the command is issued or in a directory in the network.\n");
fprintf(stream, "A relative pathname is taken relative to the directory from which the command\n");
fprintf(stream, "is issued.\n");
fprintf(stream, "\n");
fprintf(stream, "All data files restored with an instance of the dceback restoresecurity\n");
fprintf(stream, "command that includes this option are read from the single file specified with\n");
fprintf(stream, "the option. Use this option or use the -stdin option.\n");
}

void restoresecurity_stdin(stream) FILE* stream;
{
fprintf(stream, "Directs the command to read the Security Service data files it is to restore\n");
fprintf(stream, "from standard input instead of from a file. Use this option to read output\n");
fprintf(stream, "piped to the dceback restoresecurity command from the zcat program.\n");
fprintf(stream, "This option allows you to restore data files from a compressed tar file\n");
fprintf(stream, "created by piping the output of the dceback dumpsecurity command to the\n");
fprintf(stream, "compress program. Use this option or use the -sourcefile option.\n");
}

void restoresecurity_component(stream) FILE* stream;
{
fprintf(stream, "Specifies the Security Service data files to be restored from the indicated\n");
fprintf(stream, "source file. Only those files previously dumped to the source file with the\n");
fprintf(stream, "dceback dumpsecurity command can be restored. Specify one or both of the\n");
fprintf(stream, "following arguments:\n");
fprintf(stream, "\n");
fprintf(stream, "+ common\n");
fprintf(stream, "Directs the command to restore Security Service data files common to every\n");
fprintf(stream, "machine configured as a DCE client. The command restores the following files:\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/var/security/lrgy_data\n");
fprintf(stream, "A host-specific file that contains the local login cache. The file contains an\n");
fprintf(stream, "entry for each user who effectively authenticates to DCE on the machine. It\n");
fprintf(stream, "never contains more than a specified number of entries, typically 25. Each\n");
fprintf(stream, "entry contains account information that can be used to provide local\n");
fprintf(stream, "authentication in the event that a Security Server is not available. With the\n");
fprintf(stream, "lrgy_tgts file, this file constitutes a machine's local registry; the\n");
fprintf(stream, "two files may not exist on every machine.\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/var/security/lrgy_tgts\n");
fprintf(stream, "A host-specific file that contains a valid ticket-granting ticket (TGT) for\n");
fprintf(stream, "each entry in the local lrgy_data file. The information in the TGT allows\n");
fprintf(stream, "a user with an entry in the local login cache to authenticate to the local\n");
fprintf(stream, "machine in the event that a Security Server is not available. With the\n");
fprintf(stream, "lrgy_data file, this file composes a machine's local registry; the two\n");
fprintf(stream, "files may not exist on every machine.\n");
fprintf(stream, "\n");
fprintf(stream, "+ <dcelocal>/etc/security/pe_site\n");
fprintf(stream, "A host-independent file that contains information necessary for the machine to\n");
fprintf(stream, "contact the cell's master Security Server. For each protocol sequence supported\n");
fprintf(stream, "by the cell's master Security Server, the file contains a line that lists the\n");
fprintf(stream, "name of the cell to which the machine belongs and a string binding that\n");
fprintf(stream, "includes the master Security Server's protocol sequence, as follows:\n");
fprintf(stream, "   \n");
fprintf(stream, "   /.../<cell_name> <UUID>@<prot_seq>:<IP_address>[<port_number>]\n");
fprintf(stream, "\n");
fprintf(stream, "A copy of this file resides on every machine configured as a DCE client.\n");
fprintf(stream, "\n");
fprintf(stream, "+ /krb5/krb.conf\n");
fprintf(stream, "A host-independent Kerberos configuration file that contains two lines: the\n");
fprintf(stream, "first line lists the name of the cell to which the machine belongs, and the\n");
fprintf(stream, "second line lists the name of the cell to which the machine belongs, followed\n");
fprintf(stream, "by the name of the cell's master Security Server. The file has the following\n");
fprintf(stream, "format:\n");
fprintf(stream, "   \n");
fprintf(stream, "   <cell_name>\n");
fprintf(stream, "   <cell_name> <Security_Server_hostname>\n");
fprintf(stream, "\n");
fprintf(stream, "A copy of this file resides on every machine configured as a DCE client.\n");
fprintf(stream, "\n");
fprintf(stream, "+ /krb5/v5srvtab\n");
fprintf(stream, "A host-specific keytab file that contains the encryption keys (and their\n");
fprintf(stream, "version numbers) for server processes that run on the local machine. An\n");
fprintf(stream, "encryption key enables processes to communicate securely by using the key to\n");
fprintf(stream, "encrypt and decrypt messages.\n");
fprintf(stream, "\n");
fprintf(stream, "Ensure that no one is editing one of these files when you issue this command.\n");
fprintf(stream, "If a file to be restored is being edited when you issue this command, the\n");
fprintf(stream, "restored version of the file may be overwritten when the version being edited\n");
fprintf(stream, "is saved. Because any DCE process running on a machine can modify the keytab\n");
fprintf(stream, "file on that machine, you may want to stop all DCE processes on a machine\n");
fprintf(stream, "before restoring common Security Service data files, restarting the stopped\n");
fprintf(stream, "processes once the files have been restored.\n");
fprintf(stream, "\n");
fprintf(stream, "+ master\n");
fprintf(stream, "Directs the command to restore the registry database and the master key for\n");
fprintf(stream, "the database. The command restores the following files to the\n");
fprintf(stream, "<dcelocal>/var/security directory:\n");
fprintf(stream, "\n");
fprintf(stream, "+ .mkey\n");
fprintf(stream, "A file that contains the master key used to encrypt all passwords in the\n");
fprintf(stream, "registry database. Because the file contains the master key for the registry\n");
fprintf(stream, "database, the database is useless without it. The file itself is host\n");
fprintf(stream, "independent.\n");
fprintf(stream, "\n");
fprintf(stream, "+ rgy_data\n");
fprintf(stream, "A directory that contains the files that house the registry database. The\n");
fprintf(stream, "directory and its contents are host independent.\n");
fprintf(stream, "\n");
fprintf(stream, "Use the master argument to restore these files only to the machine\n");
fprintf(stream, "configured as the cell's master Security Server. Before restoring these files,\n");
fprintf(stream, "stop the secd process on the master Security Server. After restoring\n");
fprintf(stream, "the files, restart the secd process with the -restore_master\n");
fprintf(stream, "option; this option starts the secd process and instructs it to mark\n");
fprintf(stream, "replicas of the registry database that reside on machines configured as slave\n");
fprintf(stream, "Security Servers to be reinitialized.\n");
fprintf(stream, "\n");
fprintf(stream, "Each machine configured as a slave Security Server houses a replica of the\n");
fprintf(stream, "registry database and a .mkey file, which is unique for its copy of the\n");
fprintf(stream, "registry database (each .mkey file is unique for the copy of the database\n");
fprintf(stream, "that resides on its machine). Nonetheless, use the master argument to\n");
fprintf(stream, "restore these files only to the machine configured as the master Security\n");
fprintf(stream, "Server.\n");
fprintf(stream, "\n");
fprintf(stream, "Omit this option to restore all of the Security Service data files contained\n");
fprintf(stream, "in the source file. Omitting this option is equivalent to specifying it with\n");
fprintf(stream, "both of its acceptable arguments. If the source file contains the registry\n");
fprintf(stream, "database and the master key file, omit this option only if the machine to\n");
fprintf(stream, "which files are to be restored is configured as the master Security Server\n");
fprintf(stream, "for the cell.\n");
}

void restoresecurity_destdir(stream) FILE* stream;
{
fprintf(stream, "Names an alternate root directory beneath which previously dumped Security\n");
fprintf(stream, "Service data files are to be restored. The pathname of the specified directory\n");
fprintf(stream, "must be accessible from the machine indicated with the -host option. It\n");
fprintf(stream, "can resolve to a directory on the indicated machine or to a directory in the\n");
fprintf(stream, "network. You must supply a full pathname; a relative pathname is not\n");
fprintf(stream, "acceptable. The specified directory must already exist.\n");
fprintf(stream, "\n");
fprintf(stream, "By default, each data file is restored to the directory in which it typically\n");
fprintf(stream, "resides (the directory from which it was dumped). You can use this option to\n");
fprintf(stream, "specify an alternate root directory beneath which all files are to be restored.\n");
fprintf(stream, "The command creates the necessary hierarchy of subdirectories if it does not\n");
fprintf(stream, "already exist beneath the specified directory.\n");
}

void restoresecurity_help(stream) FILE* stream;
{
fprintf(stream, "Displays the online help for this command. All other valid options specified\n");
fprintf(stream, "with this option are ignored.\n");
}

