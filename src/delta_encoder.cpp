#include "karazeh/delta_encoder.hpp"

namespace kzh {

  static size_t block_len = RS_DEFAULT_BLOCK_LEN;
  static size_t strong_len = RS_DEFAULT_STRONG_LEN;

  delta_encoder::delta_encoder()
  : logger("delta_encoder[rdiff]")
  {
  }

  delta_encoder::~delta_encoder() {
  }

  rs_result delta_encoder::signature(path_t const& basis_path, path_t const& sig_path)
  {
    FILE            *basis_file, *sig_file;
    rs_stats_t      stats;
    rs_result       result;
    
    if (!rmgr_.is_readable(basis_path)) {
      throw invalid_resource("no such basis for signature: " + basis_path.string());
    }
    if (!rmgr_.is_writable(sig_path)) {
      throw invalid_state("signature destination is not writable: " + sig_path.string());
    }

    basis_file  = rs_file_open(basis_path.c_str(), "rb");
    sig_file    = rs_file_open(sig_path.c_str(), "wb");

    result = rs_sig_file(basis_file, sig_file, block_len, strong_len, &stats);

    rs_file_close(sig_file);
    rs_file_close(basis_file);

    if (result != RS_DONE)
      return result;

    return result;
  }


  rs_result delta_encoder::delta(path_t const& sig_path, path_t const &file_path, path_t const& delta_path)
  {
    FILE            *sig_file, *new_file, *delta_file;
    rs_result       result;
    rs_signature_t  *sumset;
    rs_stats_t      stats;

    if (!rmgr_.is_readable(sig_path)) {
      throw invalid_resource("signature file is unreadable: " + sig_path.string());
    }
    if (!rmgr_.is_readable(file_path)) {
      throw invalid_resource("reference file is unreadable: " + file_path.string());
    }    
    if (!rmgr_.is_writable(delta_path)) {
      throw invalid_state("delta destination is not writable: " + delta_path.string());
    }
    
    sig_file = rs_file_open(sig_path.c_str(), "rb");
    new_file = rs_file_open(file_path.c_str(), "rb");
    delta_file = rs_file_open(delta_path.c_str(), "wb");

    result = rs_loadsig_file(sig_file, &sumset, &stats);
    if (result != RS_DONE)
      return result;

    if ((result = rs_build_hash_table(sumset)) != RS_DONE)
      return result;

    result = rs_delta_file(sumset, new_file, delta_file, &stats);

    rs_free_sumset(sumset);

    rs_file_close(delta_file);
    rs_file_close(new_file);
    rs_file_close(sig_file);

    return result;
  }

  rs_result delta_encoder::patch(path_t const& basis_path, path_t const& delta_path, path_t const& out_path)
  {
    // patch BASIS [DELTA [NEWFILE]]
    FILE               *basis_file, *delta_file, *new_file;
    rs_stats_t          stats;
    rs_result           result;

    if (!rmgr_.is_readable(basis_path)) {
      throw invalid_resource("basis file is unreadable: " + basis_path.string());
    }
    if (!rmgr_.is_readable(delta_path)) {
      throw invalid_resource("delta file is unreadable: " + delta_path.string());
    }    
    if (!rmgr_.is_writable(out_path)) {
      throw invalid_state("target destination is not writable: " + out_path.string());
    }
    
    basis_file = rs_file_open(basis_path.c_str(), "rb");
    delta_file = rs_file_open(delta_path.c_str(), "rb");
    new_file =   rs_file_open(out_path.c_str(), "wb");

    result = rs_patch_file(basis_file, delta_file, new_file, &stats);

    rs_file_close(new_file);
    rs_file_close(delta_file);
    rs_file_close(basis_file);

    return result;
  }
}