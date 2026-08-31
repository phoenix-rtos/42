import os
import shutil
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import glob


def overwrite_line_in_file(filepath, line_num, new_content):
    """
    Replaces a specific line in a file with new content.
    line_num is 1-based index.
    """
    try:
        with open(filepath, "r") as f:
            lines = f.readlines()

        if 1 <= line_num <= len(lines):
            lines[line_num - 1] = new_content

            with open(filepath, "w") as f:
                f.writelines(lines)
        else:
            print(f"Warning: Line {line_num} out of range for file {filepath}")

    except Exception as e:
        print(f"Error modifying file {filepath}: {e}")


def run_mc():
    # Initialization
    NOISE_MULTIPLIER = 50.0
    base_path = "./smc"
    # type_of_run = "smc"
    # type_of_run = "pd"
    # base_run_path = "bdot_logs"  # if type_of_run == "smc" else "pd_logs"
    base_run_path = "ekf_logs"
    n_run = 59
    max_core = 1

    # Ensure BasePath exists
    if not os.path.exists(base_path):
        print(f"Error: Base path {base_path} does not exist.")
        return

    n_batch = int(np.ceil(n_run / max_core))

    # Path to BatchRun.sh
    # batch_run_script = os.path.abspath("./Utilities/BatchRun-bdot.sh")
    batch_run_script = os.path.abspath("./Utilities/BatchRun-ekf.sh")
    # batch_run_script = (
    #     os.path.abspath("./Utilities/BatchRun-smc.sh")
    #     if type_of_run == "smc"
    #     else os.path.abspath("./Utilities/BatchRun-pd.sh")
    # )

    print(f"Starting Monte Carlo Campaign: {n_run} runs in {n_batch} batches.")

    for i_batch in range(n_batch):
        # 0-based index for batch
        start_run_idx = i_batch * max_core
        # Calculate how many cores to use for this batch
        n_core = min(max_core, n_run - start_run_idx)

        batch_paths = []

        # Prepare runs for this batch
        for i_core in range(n_core):
            i_run = start_run_idx + i_core + 1  # 1-based run index
            run_path = f"{base_run_path}/Run{i_run:03d}"

            # Clean and Copy
            if os.path.exists(run_path):
                shutil.rmtree(run_path)
            shutil.copytree(base_path, run_path)
            batch_paths.append(run_path)

            # Modify Inp_Sim.txt
            # Set Random Seed
            seed = 100 * i_run
            s = f"{seed}                         ! RNG Seed\n"
            overwrite_line_in_file(os.path.join(run_path, "Inp_Sim.txt"), 6, s)

            # Make sure graphics front end is FALSE.
            s = "FALSE                           !  Graphics Front End?\n"
            overwrite_line_in_file(os.path.join(run_path, "Inp_Sim.txt"), 7, s)

            # Modify SC_Simple.txt
            # Randomize Euler Angles
            eul_ang = 20 * (2 * np.random.rand(3) - 1)
            s = f"{eul_ang[0]:.2f}  {eul_ang[1]:.2f}   {eul_ang[2]:.2f}    213      ! Angles (deg) & Euler Sequence\n"
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 16, s)

            # Randomize angular velocities (-20 to -10 or +10 to +20 deg/sec)
            ang_vel = np.zeros(3)
            for i in range(3):
                if np.random.rand() < 0.5:
                    ang_vel[i] = -1.0 + 2.0 * np.random.rand()
                else:
                    ang_vel[i] = 14.0 + 2.0 * np.random.rand()
            s = f"{ang_vel[0]:.2f}  {ang_vel[1]:.2f}   {ang_vel[2]:.2f}      ! Ang Vel (deg/sec)\n"
            # overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 14, s)

            # Randomize inclination (70-89 degrees)
            inc = 70.0 + 19.0 * np.random.rand()
            s = f"{inc:.2f}                 ! Inclination (deg)\n"
            # overwrite_line_in_file(os.path.join(run_path, "Orb_LEO.txt"), 17, s)

            def randomize_inertia():
                # Randomize inertia matrix (40% variation)
                J_nom = np.array([0.03, 0.06, 0.085])
                if i_run == 1:
                    J_new = J_nom
                elif i_run == 2:
                    J_new = J_nom * 1.4
                elif i_run == 3:
                    J_new = J_nom * 0.6
                else:
                    uncertainty_diag = np.random.uniform(-0.2, 0.2, size=3)
                    J_new = J_nom * (1.0 + uncertainty_diag)
                s = f"{J_new[0]:.6f}   {J_new[1]:.6f}   {J_new[2]:.6f}      ! Moments of Inertia (kg-m^2)\n"
                overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 31, s)

                # Generate Random off-diagonals (line 32):
                # 0.0  0.0  0.0                 ! Products of Inertia (xy,xz,yz)
                # 3% of max diagonal value
                max_offdiag = 0.03 * np.max(J_new)
                off_diag = np.random.uniform(-max_offdiag, max_offdiag, size=3)
                # Verify positive definite
                J_matrix = np.array(
                    [
                        [J_new[0], off_diag[0], off_diag[1]],
                        [off_diag[0], J_new[1], off_diag[2]],
                        [off_diag[1], off_diag[2], J_new[2]],
                    ]
                )
                if np.all(np.linalg.eigvals(J_matrix) > 0):
                    s = f"{off_diag[0]:.6f}   {off_diag[1]:.6f}   {off_diag[2]:.6f}      ! Products of Inertia (xy,xz,yz)\n"
                    overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 32, s)
                    return 0
                else:
                    return -1

            # Randomize Inertia until positive definite
            # while randomize_inertia() != 0:
            # pass

            # Increase noise
            base_gyro_noise = 0.025
            base_mag_noise = 25.0e-9
            base_sun_noise = 0.5

            gyro_noise = base_gyro_noise * NOISE_MULTIPLIER
            mag_noise = base_mag_noise * NOISE_MULTIPLIER
            sun_noise = base_sun_noise * NOISE_MULTIPLIER

            # 0.025                       ! Angle Random Walk (deg/rt-hr)

            s = f"{gyro_noise:.6f}                       ! Angle Random Walk (deg/rt-hr)\n"
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 117, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 128, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 139, s)

            # 15.0E-9                     ! Noise, Tesla RMS
            s = f"{mag_noise:.6e}                     ! Noise, Tesla RMS\n"
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 152, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 160, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 168, s)

            # 0.5                         ! Noise Equivalent Angle, deg RMS
            s = f"{sun_noise:.6f}                         ! Noise Equivalent Angle, deg RMS\n"
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 187, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 195, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 203, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 211, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 219, s)
            overwrite_line_in_file(os.path.join(run_path, "SC_Simple.txt"), 227, s)

        # Perform Runs
        print(f"Starting Runs {start_run_idx + 1} - {start_run_idx + n_core}")

        # BatchRun.sh expects paths as arguments
        cmd = [batch_run_script] + batch_paths
        print(f"Executing command: {' '.join(cmd)}")
        try:
            subprocess.run(cmd, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error executing batch run: {e}")
            continue

        # Process Batch Results
        for i_core in range(n_core):
            i_run = start_run_idx + i_core + 1
            run_path = f"{base_run_path}/Run{i_run:03d}"

            # Cleanup files
            save_list = ["state"]  # bdot
            # save_list = ["wbn", "MTB"]  # ekf
            for name in save_list:
                src = os.path.join(run_path, f"{name}.42")
                dst = os.path.join(run_path, f"{name}.tmp")
                if os.path.exists(src):
                    shutil.move(src, dst)

            # Remove all .42 files
            for f in glob.glob(os.path.join(run_path, "*.42")):
                os.remove(f)

            for f in glob.glob(os.path.join(run_path, "*.csv")):
                os.remove(f)

            # Restore saved files
            for name in save_list:
                src = os.path.join(run_path, f"{name}.tmp")
                dst = os.path.join(run_path, f"{name}.42")
                if os.path.exists(src):
                    shutil.move(src, dst)

            # Delete txt files
            files_to_delete = [
                "Flex_Simple.txt",
                "Inp_Cmd.txt",
                "Inp_FOV.txt",
                "Inp_Graphics.txt",
                "Inp_IPC.txt",
                "Inp_NOS3.txt",
                "Inp_Region.txt",
                "Inp_TDRS.txt",
                "Orb_LEO.txt",
                "SC_SensorFOV.txt",
                "Shaker_Simple.txt",
                "TRV.txt",
                "Inp_AcOutput.txt",
                "Inp_ScOutput.txt",
                "Nodes_Simple.txt",
                "Optics_Simple.txt",
                "Orb_L3.txt",
                "Whl_Simple.txt",
            ]
            for fname in files_to_delete:
                fpath = os.path.join(run_path, fname)
                if os.path.exists(fpath):
                    os.remove(fpath)


if __name__ == "__main__":
    run_mc()
