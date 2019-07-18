<?php

use Symfony\Component\Dotenv\Dotenv;
use Symfony\Component\Finder\Finder;
use Symfony\Component\Filesystem\Filesystem;

/**
 * Class RoboFile
 */
class RoboFile extends \Robo\Tasks {
    /**
     * @var string Defines the root of the parent composer project.
     */
    public $projectRootPath;

    /**
     * @var array A list of valid environment variables for this project.
     */
    public $requiredEnvVars;

    /**
     * @var array A nested array of files and folders to be synced.
     *
     * Valid keys are 'files' and 'folders'. Below each key is the 'src' and 'dest' for the sync operation.
     */
    public $syncPaths;

    /**
     * @var array A nested array of files and folders to be parsed for project variable replacements.
     *
     * Valid keys are 'files'. 'folders' and 'exclude'. Each key contains an array of strings, representing file or
     * folder paths. Note in order to limit the replacement scope folders are NOT recursed. You will need to include
     * all folders that need to be searched.
     *
     */
    public $replaceFiles;

    /**
     * @var Filesystem Filesystem object for general operations.
     */
    protected $fs;

    /**
     * RoboFile constructor.
     */
    function __construct() {
        $this->fs = new Filesystem();
        $this->setProjectRootPath();
        $this->loadProjectConfig();
        $this->loadEnvVariables();
    }

    /**
     * Copies all template files to the project directory and replaces placeholders.
     */
    public function sync() {
        $this->copyTemplates();
        $this->replaceEnvVars();
    }

    /**
     * Copy template files to project.
     *
     * @return void
     */
    function copyTemplates() {
        // Copy directories to project.
        foreach ($this->syncPaths['folders'] as $src => $dest) {
            $this->say('Syncing ' . $src . ' to ' . $dest);
            $this->fs->mirror($src, $this->getProjectRootPath() . $dest, NULL, array('override' => TRUE));
        }

        // Copy template files to project.
        foreach ($this->syncPaths['files'] as $src => $dest) {
            $this->fs->copy($src, $dest, true);
        }
    }

    /**
     * Check that expected environmental variables are defined in the project.
     *
     * @return bool
     */
    protected function validEnvVars() {
        foreach ($this->requiredEnvVars as $var) {
            if (!isset($_ENV[$var])) {
                return FALSE;
            }
        }
        return TRUE;
    }

    /**
     * Replace project level variables.
     *
     * @param Finder $files A finder object containing the file list to scan for replacements.
     */
    protected function replaceEnvVars() {
        foreach ($this->replaceFiles as $file) {
            $file = $this->getProjectRootPath() . $file;

            if (!$this->fs->exists($file)) {
                $this->say($file . ' does not exist. Skipping.');
                continue;
            }
            foreach ($this->requiredEnvVars as $envVar) {
                $this->say('Checking ' . $file . ' for ' . $envVar);
                $this->taskReplaceInFile($file)
                    ->from('%%' . $envVar . '%%')
                    ->to($_ENV[$envVar])
                    ->run();
            }
        }
    }

    /**
     * Loads environment variables set in .env file into the PHP $_ENV superglobal. Exits with a message if no .env file
     * exists.
     */
    private function loadEnvVariables() {
        $dotenv = new Dotenv();
        try {
            $dotenv->load($this->getProjectRootPath() . 'project.env');
        }
        catch(Exception $e) {
            die('Error loading .env variables. Ensure you have a .env file in your project root.');
        }

        if (!$this->validEnvVars()) {
            die('Check your .env file - you seem to be missing a required variable. Required variables are ' . implode(', ', $this->requiredEnvVars));
        }
    }

    /**
     * Sets the configuration for the project. This includes expected environmental variables, folder paths to be
     * synced, and filepaths to be processed by the replacement process.
     */
    private function loadProjectConfig() {

        $this->requiredEnvVars = array(
            'PROJECT',
            'PROJECT_NAME'
        );

        $this->syncPaths = array(
            'folders' => array(
                'dist/.circleci' => '.circleci',
                'dist/.docker' => '.docker',
                'dist/.github' => '.github',
                'dist/config' => 'config',
                'dist/docroot' => 'docroot',
                'dist/dpc-sdp' => 'dpc-sdp',
                'dist/drush' => 'drush',
                'dist/patches' => 'patches',
                'dist/scripts' => 'scripts',
                'dist/root' => '.'
            ),
            'files' => array(),
        );

        $this->replaceFiles = array(
            'README.md',
            '.env'
        );
    }

    /**
     * Gets the root project path.
     *
     * @return string
     */
    private function getProjectRootPath() {
        return $this->projectRootPath;
    }

    /**
     * Sets the root project path. This will differ when running this project within another composer project.
     */
    private function setProjectRootPath() {
        if (basename(dirname(__DIR__)) == 'dpc-sdp') {
            $this->projectRootPath = (dirname(__DIR__, 3) . '/');
        }
        else {
            $this->projectRootPath = __DIR__ . '/';
        }
    }
}
